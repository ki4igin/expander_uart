#include "aura.h"
#include "chunk.h"
#include "uid_hash.h"
#include "assert.h"
#include "crc16.h"
#include "usart_ex.h"
#include "send_fifo.h"
#include "dict.h"
#include "gpio.h"
#include "relay.h"
#include "sens.h"
#include "bat.h"
#include "stm32f4xx_ll_tim.h"

#define AURA_PROTOCOL      0x41525541U
#define AURA_MAX_REPEATERS 2
#define AURA_MAX_DATA_SIZE 128

static dict_declare(map, AURA_MAX_REPEATERS *(UART_COUNT - 1));

#define map ((struct dict *)map_buf)

struct send_fifo send_fifo;

enum state_recv {
    STATE_RECV_START = 0,
    STATE_RECV_HEADER,
};

enum cmd {
    CMD_NONE = 0,
    CMD_REQ_WHOAMI = 1,
    CMD_ANS_WHOAMI = 2,
    CMD_REQ_DATA = 3,
    CMD_ANS_DATA = 4,
    CMD_REQ_WRITE = 5,
    CMD_ANS_WRITE = 6,
    CMD_REQ_READ = 7,
    CMD_ANS_READ = 8,
};

enum device_type {
    DEVICE_TYPE_LB75BD = 1,
    DEVICE_TYPE_TMP112,
    DEVICE_TYPE_SHT30,
    DEVICE_TYPE_ZS05,
    DEVICE_TYPE_BMP180,
    DEVICE_TYPE_LPS22HB,
    DEVICE_TYPE_DOORKNOT,
    DEVICE_TYPE_EXPANDER,
    DEVICE_TYPE_WETSENS,
};

struct header {
    const uint32_t protocol;
    uint32_t cnt;
    uint32_t uid_src;
    uint32_t uid_dest;
    uint16_t cmd;
    uint16_t data_sz;
};

struct __PACKED pack {
    struct header header;
    uint8_t data[AURA_MAX_DATA_SIZE];
    crc16_t crc;
};

static struct __PACKED pack pack_ans = {
    .header = {.protocol = AURA_PROTOCOL},
};

static enum state_recv states_recv[UART_COUNT] = {0};
static struct pack packs[UART_COUNT] __ALIGNED(8);
static uint32_t aura_flags_pack_received[UART_COUNT] = {0};
static uint32_t aura_flag_send_delay = 0;
static uint32_t cnt_send_pack = 0;

static void aura_recv_package(uint32_t num)
{
    states_recv[num] = STATE_RECV_START;
    struct uart *u = &uarts[num];
    struct pack *p = &packs[num];
    uart_recv_array(u, p, sizeof(struct header));
}

static void cmd_write_data(const struct pack *req, void **next_ans_chunk)
{
    int32_t req_data_size = req->header.data_sz;
    void *next_req_chunk = (void *)req->data;

    while (req_data_size > 0) {
        struct chunk_hdr *hdr = (struct chunk_hdr *)next_req_chunk;
        uint32_t chunk_size = hdr->size + sizeof(struct chunk_hdr);
        next_req_chunk = (void *)((uint32_t)next_req_chunk + chunk_size);
        req_data_size -= chunk_size;

        switch (hdr->id) {
        case CHUNK_ID_RELAY1_STATUS:
        case CHUNK_ID_RELAY2_STATUS: {
            enum relay relay = (hdr->id == CHUNK_ID_RELAY1_STATUS) ? RELAY1 : RELAY2;
            struct chunk_u16 *c = (struct chunk_u16 *)hdr;
            if (c->val == 0x00FF) {
                relay_open(relay);
            } else if (c->val == 0x0000) {
                relay_close(relay);
            }
            uint16_t data = relay_is_open(relay) ? 0x00FF : 0x0000;
            chunk_u16_add(next_ans_chunk, hdr->id, data);
        } break;
        default: {
        } break;
        }
    }
}

static void cmd_work_master()
{
    if (aura_flags_pack_received[0] == 0) {
        return;
    }
    LL_TIM_SetCounter(TIM7, 0);
    aura_flag_send_delay = 1;
    aura_flags_pack_received[0] = 0;
    
    struct pack *req = &packs[0];
    uint32_t pack_size = sizeof(req->header)
                       + req->header.data_sz
                       + sizeof(req->crc);
    if (req->header.uid_dest == 0) {
        for (uint32_t i = 1; i < UART_COUNT; i++) {
            uart_send_array(&uarts[i], req, pack_size);
        }
    } else {
        uint32_t idx = dict_get_idx(map, req->header.uid_dest);
        if (idx != -1U) {
            uint32_t uart_num = map->kvs[idx].value;
            uart_send_array(&uarts[uart_num], req, pack_size);
        }
    }
    if ((req->header.uid_dest != 0)
        && (req->header.uid_dest != pack_ans.header.uid_src)) {
        return;
    }

    struct pack *ans = &pack_ans;
    ans->header.cnt = cnt_send_pack++;
    ans->header.uid_dest = req->header.uid_src;
    ans->header.cmd = CMD_NONE;

    void *next_ans_chunk = ans->data;

    switch (req->header.cmd) {
    case CMD_REQ_WHOAMI: {
        dict_clear(map);
        ans->header.cmd = CMD_ANS_WHOAMI;
        chunk_u32_add(&next_ans_chunk, CHUNK_ID_TYPE, DEVICE_TYPE_EXPANDER);
    } break;
    case CMD_REQ_DATA: {
        ans->header.cmd = CMD_ANS_DATA;
        chunk_u16_add(&next_ans_chunk, CHUNK_ID_WETSENS, sens_get_state());
        chunk_u16_add(&next_ans_chunk, CHUNK_ID_BAT_VOLT, bat_get_voltage());
        chunk_u16_add(&next_ans_chunk, CHUNK_ID_RELAY1_STATUS,
                      relay_is_open(RELAY1) ? 0x00FF : 0x0000);
        chunk_u16_add(&next_ans_chunk, CHUNK_ID_RELAY2_STATUS,
                      relay_is_open(RELAY2) ? 0x00FF : 0x0000);

    } break;
    case CMD_REQ_WRITE: {
        ans->header.cmd = CMD_ANS_WRITE;
        cmd_write_data(req, &next_ans_chunk);
    } break;
    default: {
    } break;
    }

    ans->header.data_sz = (uint32_t)next_ans_chunk - (uint32_t)ans->data;
    uint32_t pack_ans_size = sizeof(ans->header)
                           + ans->header.data_sz
                           + sizeof(pack_ans.crc);
    crc16_add2pack(ans, pack_ans_size);
    send_fifo_push(&send_fifo, ans, pack_ans_size);
}

static void cmd_work_slave(uint32_t num)
{
    if (aura_flags_pack_received[num] == 0) {
        return;
    }

    struct pack *p = &packs[num];

    switch (p->header.cmd) {
    case CMD_ANS_WHOAMI: {
        dict_add(map, p->header.uid_src, num);
        struct chunk_u32 *c = (struct chunk_u32 *)&p->data;
        c++;
        if (p->header.data_sz == sizeof(struct chunk_u32)) {
            p->header.data_sz += sizeof(struct chunk_u32);
            c->hdr.id = CHUNK_ID_UIDS;
            c->hdr.type = CHUNK_TYPE_ARR_U32,
            c->hdr.size = sizeof(struct chunk_u32);
            c->val = pack_ans.header.uid_src;
        } else {
            p->header.data_sz += sizeof(uint32_t);
            struct chunk_u32arr *c_arr = (struct chunk_u32arr *)c;
            uint32_t uids_count = c->hdr.size / sizeof(uint32_t);
            c->hdr.size += sizeof(uint32_t);
            c_arr->arr[uids_count] = pack_ans.header.uid_src;
        }
        uint32_t pack_size = sizeof(struct header)
                           + p->header.data_sz
                           + sizeof(crc16_t);
        crc16_add2pack(p, pack_size);
        send_fifo_push(&send_fifo, p, pack_size);
    } break;
    default: {
        uint32_t pack_size = sizeof(struct header)
                           + p->header.data_sz
                           + sizeof(crc16_t);
        send_fifo_push(&send_fifo, p, pack_size);
    } break;
    }
    aura_flags_pack_received[num] = 0;
}

static void send_resp_data()
{
    if (send_fifo_is_empty(&send_fifo)) {
        return;
    }
    if (uarts[0].tx.count != 0) {
        return;
    }
    if (aura_flag_send_delay){
        return;
    }
    // taking data from fifo
    struct pack *p = (struct pack *)send_fifo_get_ptail(&send_fifo);

    uint32_t pack_size = sizeof(struct header)
                       + p->header.data_sz
                       + sizeof(crc16_t);
    
    send_fifo_inc_tail(&send_fifo, pack_size);
    uart_send_array(&uarts[0], p, pack_size);
}

void tim7_update_callback()
{
    aura_flag_send_delay = 0;
}

void aura_process(void)
{
    cmd_work_master();
    for (uint32_t i = 1; i < UART_COUNT; i++) {
        cmd_work_slave(i);
    }
    send_resp_data();
}

void aura_init(void)
{
    uint32_t uid = uid_hash();
    pack_ans.header.uid_src = uid;
    aura_recv_package(0);
}

void uart_recv_complete_callback(struct uart *u)
{
    uint32_t num = u->num;
    enum state_recv *s = &states_recv[num];
    struct pack *p = &packs[num];

    switch (*s) {
    case STATE_RECV_START: {
        *s = STATE_RECV_HEADER;

        if (p->header.data_sz > sizeof(p->data)) {
            p->header.data_sz = 0;
        }

        uart_recv_array(u,
                        p->data,
                        p->header.data_sz + sizeof(crc16_t));
    } break;
    case STATE_RECV_HEADER: {
        uint32_t pack_size = sizeof(struct header)
                           + p->header.data_sz
                           + sizeof(crc16_t);
        if (crc16_is_valid(p, pack_size)) {
            aura_flags_pack_received[num] = 1;
        }
        aura_recv_package(num);
    } break;
    }
}

void uart_send_complete_callback(struct uart *u)
{
    aura_recv_package(u->num);
}

void uart_recv_timeout_callback(struct uart *u)
{
    uart_stop_recv(u);
    if (u->num == 0) {
        aura_recv_package(0);
    }
}
