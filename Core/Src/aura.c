#include "aura.h"
#include "uid_hash.h"
#include "assert.h"
#include "crc16.h"
#include "usart_ex.h"
#include "fifo.h"
#include "dict.h"
#include "adc_ex.h"
#include "gpio.h"

#define AURA_PROTOCOL               0x41525541U
#define AURA_MAX_REPEATERS          2
#define AURA_EXPANDER_ID            8
#define AURA_MAX_DATA_SIZE          128
#define AURA_CHUNK_HDR_SIZE         4
#define AURA_HDR_SIZE               20

#define AURA_MAX_DEVICES            8

#define AURA_SENS_CNT               8
#define AURA_RELAY_CNT              2

static dict_declare(map, AURA_MAX_REPEATERS * (UART_COUNT - 1));
static dict_declare(map, 8 * 8);

#define map ((struct dict *)map_buf)

struct fifo send_fifo;

enum state_recv {
    STATE_RECV_START = 0,
    STATE_RECV_HEADER,    
};

enum chunk_type {
    CHUNK_TYPE_NONE = 0,
    CHUNK_TYPE_I8 = 1,
    CHUNK_TYPE_U8 = 2,
    CHUNK_TYPE_I16 = 3,
    CHUNK_TYPE_U16 = 4,
    CHUNK_TYPE_I32 = 5,
    CHUNK_TYPE_U32 = 6,
    CHUNK_TYPE_F32 = 7,
    CHUNK_TYPE_F64 = 8,
    CHUNK_TYPE_STR = 9,
    CHUNK_TYPE_ARR_I8 = 10,
    CHUNK_TYPE_ARR_U8 = 11,
    CHUNK_TYPE_ARR_I16 = 12,
    CHUNK_TYPE_ARR_U16 = 13,
    CHUNK_TYPE_ARR_I32 = 14,
    CHUNK_TYPE_ARR_U32 = 15,
    CHUNK_TYPE_ARR_F32 = 16,
    CHUNK_TYPE_ARR_F64 = 17,
};

enum cmd {
    CMD_REQ_WHOAMI = 1,
    CMD_ANS_WHOAMI = 2,
    CMD_REQ_DATA = 3,
    CMD_ANS_DATA = 4,
    CMD_REQ_WRITE = 5,
    CMD_ANS_WRITE = 6,
    CMD_REQ_READ = 7,
    CMD_ANS_READ = 8,
};

enum chunk_id {
    CHUNK_ID_TYPE = 1,
    CHUNK_ID_UIDS = 2,
    CHUNK_ID_ONOFF = 3,
    CHUNK_ID_TEMP = 4,
    CHUNK_ID_HUM = 5,
    CHUNK_ID_PRESS = 6,
    CHUNK_ID_WETSENS = 7,
    CHUNK_ID_VOLT = 8,
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

struct chunk_hdr {
    uint8_t id;
    uint8_t type;
    uint16_t size;
};

struct chunk_u32 {
    struct chunk_hdr hdr;
    uint32_t val;
};

struct chunk_u32arr {
    struct chunk_hdr hdr;
    uint32_t arr[];
};

struct chunk_f32 {
    struct chunk_hdr hdr;
    float val;
};

struct chunk_u16 {
    struct chunk_hdr hdr;
    uint16_t val;
};

struct chunk {
    uint8_t id;
    uint8_t type;
    uint16_t size;
    uint8_t data[];
};

struct pack {
    struct header header;

    // union {
    //     struct chunk chunk[AURA_MAX_CHUNK_CNT];
    //     uint8_t raw_data[AURA_MAX_DATA_SIZE];
    // } data;

    uint8_t data[AURA_MAX_DATA_SIZE];
    crc16_t crc;
};

// clang-format off
static struct __PACKED pack_whoami {
    struct header header;
    struct chunk_u32 device_type;
    crc16_t crc;
} pack_whoami = {
    .header = {
        .protocol = AURA_PROTOCOL,
        .cmd = CMD_ANS_WHOAMI,
        .data_sz = sizeof(struct chunk_u32),
    },
    .device_type = {
        .hdr = {
            .id = CHUNK_ID_TYPE,
            .type = CHUNK_TYPE_U32,
            .size = sizeof(uint32_t),
        },
        .val = DEVICE_TYPE_EXPANDER,
    },
};

static struct __PACKED pack_state {
    struct header header;
    struct chunk_u16 sensors;
    struct chunk_f32 battery;
    struct chunk_u16 relays[2];
    crc16_t crc;
} pack_state = {
    .header = {
        .protocol = AURA_PROTOCOL,
        .uid_dest = AURA_PC_ID,
        .cmd = CMD_ANS_DATA,
        .data_sz = sizeof(struct chunk_u16) 
                + sizeof(struct chunk_f32) 
                + sizeof(struct chunk_u16),
    },
    .sensors = {
        .hdr = {
            .id = CHUNK_ID_WETSENS,
            .type = CHUNK_TYPE_U16,
            .size = sizeof(uint16_t),
        },
    },
    .battery = {
        .hdr = {
            .id = CHUNK_ID_VOLT,
            .type = CHUNK_TYPE_F32,
            .size = sizeof(float),
        },
    },
    .relays[0] = {
        .hdr = {
            .id = CHUNK_ID_ONOFF,
            .type = CHUNK_TYPE_ARR_U16,
            .size = sizeof(uint16_t),
        },
    },
    .relays[1] = {
        .hdr = {
            .id = CHUNK_ID_ONOFF,
            .type = CHUNK_TYPE_ARR_U16,
            .size = sizeof(uint16_t),
        },
    },
};
// clang-format on

static enum state_recv states_recv[UART_COUNT] = {0};
static struct pack packs[UART_COUNT] __ALIGNED(8);
static uint32_t aura_flags_pack_received[UART_COUNT] = {0};
static uint32_t cnt_send_pack = 0;

static void aura_recv_package(uint32_t num)
{
    states_recv[num] = STATE_RECV_START;
    struct uart *u = &uarts[num];
    struct pack *p = &packs[num];
    uart_recv_array(u, p, sizeof(struct header));
}

inline static uint32_t relay_is_open(uint32_t num)
{
    switch(num)
    {
        case RELAY_1:
            return LL_GPIO_IsOutputPinSet(GPIO_RELAY, GPIO_PIN_RELAY_1);
        case RELAY_2:
            return LL_GPIO_IsOutputPinSet(GPIO_RELAY, GPIO_PIN_RELAY_2);
    };
    return 3;
}

static void upd_state()
{
    struct pack_state *p = &pack_state;
    int16_t measurements[ADC_CH_CNT] = {0};
    adc_meas(measurements);
    
    p->battery.val = adc_convert_to_voltage(measurements[ADC_CH_BAT]);
    p->sensors.val = adc_get_sens_state(measurements);
    
    p->relays[0].val = relay_is_open(1) ? 0x00FF : 0x0000;
    p->relays[1].val = relay_is_open(2) ? 0x00FF : 0x0000;
}

static void cmd_work_master()
{
    if (aura_flags_pack_received[0] == 0) {
        return;
    }
    aura_flags_pack_received[0] = 0;

    struct pack *p = &packs[0];
    uint32_t pack_size = sizeof(struct header)
                       + p->header.data_sz
                       + sizeof(crc16_t);
    if (p->header.uid_dest == 0) {
        for (uint32_t i = 1; i < UART_COUNT; i++) {
            uart_send_array(&uarts[i], p, pack_size);
        }
    } else {
        uint32_t idx = dict_get_idx(map, p->header.uid_dest);
        if (idx != -1U) {
            uint32_t uart_num = map->kvs[idx].value;
            uart_send_array(&uarts[uart_num], p, pack_size);
        }
    }
    if ((p->header.uid_dest != 0)
        && (p->header.uid_dest != pack_whoami.header.uid_src)) {
        return;
    }

    switch (p->header.cmd) {
    case CMD_REQ_WHOAMI: {
        dict_clear(map);
        struct pack_whoami *pnt = &pack_whoami;
        pnt->header.cnt = cnt_send_pack++;
        pnt->header.uid_dest = p->header.uid_src;
        crc16_add2pack(pnt, sizeof(struct pack_whoami));
        fifo_push(&send_fifo, pnt, sizeof(struct pack_whoami));
    } break;
    case CMD_REQ_DATA: {
        // TODO send state of relays & wet sensors
        struct pack_state *pnt = &pack_state;
        pnt->header.cnt++;
        pnt->header.uid_dest = p->header.uid_src;
        crc16_add2pack(pnt, sizeof(struct pack_state));
        fifo_push(&send_fifo, pnt, sizeof(struct pack_state));

    } break;
    default: {
    } break;
    }
}

static void cmd_work_slave(uint32_t num)
{
    if (aura_flags_pack_received[num] == 0) {
        return;
    }
    struct pack *p = &packs[num];

    // adding new chunk
    switch (p->header.cmd) {
    case CMD_ANS_WHOAMI: {
        // adding new chunk
        // uint32_t uid = pack_whoami.header.uid_src;
        dict_add(map, p->header.uid_src, num);
        struct chunk_u32 *c = (struct chunk_u32 *)&p->data;
        c++;
        if (p->header.data_sz == sizeof(struct chunk_u32)) {
            p->header.data_sz += sizeof(struct chunk_u32);
            c->hdr.id = CHUNK_ID_UIDS;
            c->hdr.type = CHUNK_TYPE_ARR_U32,
            c->hdr.size = sizeof(struct chunk_u32);
            c->val = pack_whoami.header.uid_src;
        } else {
            p->header.data_sz += sizeof(uint32_t);
            struct chunk_u32arr *c_arr = (struct chunk_u32arr *)c;
            uint32_t uids_count = c->hdr.size / sizeof(uint32_t);
            c->hdr.size += sizeof(uint32_t);
            c_arr->arr[uids_count] = pack_whoami.header.uid_src;
        }
        uint32_t pack_size = sizeof(struct header)
                           + p->header.data_sz
                           + sizeof(crc16_t);
        crc16_add2pack(p, pack_size);
        fifo_push(&send_fifo, p, pack_size);

        // uint32_t uid_size = sizeof(uid);

        // struct chunk *c = p->data.chunk;
        // device_ids[num] = p->header.uid_src;

        // uint32_t bias = c->size;
        // c += bias; // adding bias of first pack

        // if (c->id == CHUNK_ID_UIDS) {
        //     p->header.data_sz += uid_size;
        //     uint32_t pack_size = sizeof(struct header)
        //                        + AURA_CHUNK_HDR_SIZE
        //                        + bias;
        //     fifo_push(&send_fifo, p, pack_size);

        //     pack_size = AURA_CHUNK_HDR_SIZE
        //               + c->size;
        //     fifo_push(&send_fifo, c, pack_size);
        // } else {
        //     p->header.data_sz += AURA_CHUNK_HDR_SIZE + uid_size;
        //     uint32_t pack_size = sizeof(struct header)
        //                        + AURA_CHUNK_HDR_SIZE
        //                        + bias;
        //     fifo_push(&send_fifo, p, pack_size);

        //     struct chunk new_c = {0};

        //     new_c.id = CHUNK_ID_UIDS;
        //     new_c.type = CHUNK_TYPE_ARR_U32;
        //     new_c.size = uid_size;
        //     fifo_push(&send_fifo, &new_c, AURA_CHUNK_HDR_SIZE);
        // }
        // fifo_push(&send_fifo, &uid, uid_size);

        // struct pack *p_to_crc = (struct pack *)fifo_get_ptail(&send_fifo);
        // uint16_t crc = crc16_calc(p_to_crc, sizeof(struct header)
        //                                         + p_to_crc->header.data_sz);
        // // pushing crc into fifo
        // fifo_push(&send_fifo, &crc, sizeof(crc16_t));
    } break;
    default: {
        // pushing hdr into fifo
        uint32_t pack_size = sizeof(struct header)
                           + p->header.data_sz
                           + sizeof(crc16_t);
        fifo_push(&send_fifo, p, pack_size);
        // fifo_push(&send_fifo, &p->crc, sizeof(crc16_t));
    } break;
    }
    aura_flags_pack_received[num] = 0;
}

static void send_resp_data()
{
    if (fifo_is_empty(&send_fifo)) {
        return;
    }
    if (uarts[0].tx.count != 0) {
        return;
    }
    // taking data from fifo
    struct pack *p = (struct pack *)fifo_get_ptail(&send_fifo);

    uint32_t pack_size = sizeof(struct header)
                       + p->header.data_sz
                       + sizeof(crc16_t);
    fifo_inc_tail(&send_fifo, pack_size);
    uart_send_array(&uarts[0], p, pack_size);
}

void aura_process(void)
{
    upd_state();
    cmd_work_master();
    for (uint32_t i = 1; i < UART_COUNT; i++) {
        cmd_work_slave(i);
    }
    send_resp_data();
}

void aura_init(void)
{
    pack_whoami.header.uid_src = uid_hash();
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
    // uart_stop_recv();
    // aura_recv_package(0);
}
