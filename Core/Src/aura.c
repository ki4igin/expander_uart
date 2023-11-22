#include "aura.h"
#include "uid_hash.h"
#include "assert.h"
#include "crc16.h"
#include "usart_ex.h"
#include "fifo.h"

#define AURA_PROTOCOL      0x41525541U
#define AURA_MAX_REPEATERS 4
#define AURA_EXPANDER_ID   8
#define AURA_MAX_DATA_SIZE 128
#define AURA_MAX_DATA_SIZE_IN_CHUNK 16
#define AURA_MAX_CHUNK_CNT 18

#define AURA_CHUNK_HDR_SIZE 4

struct fifo send_fifo;

enum state_recv {
    STATE_RECV_START = 0,
		STATE_OVERALL_SIZE_CHECKED,
    STATE_RECV_CHUNK_HDR,
		STATE_RECV_CHUNK_DATA,
		STATE_CHUNK_AMOUNT_CHECKED,
    STATE_RECV_CRC,
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
};

enum chunk_id {
    CHUNK_ID_WHOIM = 0,
    CHUNK_ID_STATUS = 1,
};

struct header {
    const uint32_t protocol;
    uint32_t cnt;
    uint32_t uid_src;
    uint32_t uid_dest;
		uint32_t cmd : 16;
		uint32_t data_sz : 16;
};

struct chunk {
    uint8_t id;
    uint8_t type;
    uint16_t size;
		uint8_t data[AURA_MAX_DATA_SIZE_IN_CHUNK];
};

struct data_whoami {
    uint32_t id_handle;
    uint32_t uid_repeaters[AURA_MAX_REPEATERS];
};

struct pack {
    struct header header;
    struct chunk chunk[AURA_MAX_CHUNK_CNT];
    //uint8_t data[AURA_MAX_DATA_SIZE];
    crc16_t crc;
};

// clang-format off
static struct pack_whoami {
    struct header header;
    struct chunk chunk;
    struct data_whoami data;
    crc16_t crc;
} pack_whoami = {
    .header = {
        .protocol = AURA_PROTOCOL,
    },
    .chunk = {
        .id = CHUNK_ID_WHOIM,
        .type = CHUNK_TYPE_U32,
        .size = sizeof(struct data_whoami),        
    },
};

// clang-format on

static uint32_t uid = 0;

static enum state_recv states_recv[UART_COUNT] = {0};
static struct pack packs[UART_COUNT] __ALIGNED(8);
static uint32_t aura_flags_pack_received[UART_COUNT] = {0};

static void aura_recv_package(uint32_t num)
{
    states_recv[num] = STATE_RECV_START;
    struct uart *u = &uarts[num];
    struct pack *p = &packs[num];
    uart_recv_array(u, p, sizeof(struct header));
}

static void cmd_work_master()
{
    if (aura_flags_pack_received[0] == 0) {
        return;
    }

    struct pack *p = &packs[0];
    if (p->header.uid_dest == 0) {
        uint32_t pack_size = sizeof(struct header)
                           + sizeof(struct chunk)
                           + p->chunk.size
                           + sizeof(crc16_t);
        for (uint32_t i = 1; i < UART_COUNT; i++) {
            uart_send_array(&uarts[i], p, pack_size);
        }
    }
    struct chunk *c = &p->chunk;
    switch (c->id) {
    case CHUNK_ID_WHOIM: {
        pack_whoami.header.cnt = p->header.cnt;
        crc16_add2pack(&pack_whoami, sizeof(pack_whoami));
        fifo_push(&send_fifo, p, sizeof(pack_whoami));
    } break;
    default: {
    } break;
    }
    aura_flags_pack_received[0] = 0;
}

static void cmd_work_slave(uint32_t num)
{
    if (aura_flags_pack_received[num] == 0) {
        return;
    }

    struct pack *p = &packs[num];
    uint32_t pack_size = sizeof(struct header)
                       + sizeof(struct chunk)
                       + p->chunk.size
                       + sizeof(crc16_t);

    fifo_push(&send_fifo, p, pack_size);
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

    struct pack *p = (struct pack *)fifo_get_ptail(&send_fifo);
    uint32_t pack_size = sizeof(struct header)
                       + sizeof(struct chunk)
                       + p->chunk.size
                       + sizeof(crc16_t);
    fifo_inc_tail(&send_fifo, pack_size);
    if ((p->header.uid_src != pack_whoami.header.uid_src)
        && (p->chunk.id == CHUNK_ID_WHOIM)) {
        for (uint32_t i = 0; i < AURA_MAX_REPEATERS; i++) {
            struct data_whoami *d = (struct data_whoami *)p->chunk.data;
            if (d->uid_repeaters[i] == 0) {
                d->uid_repeaters[i] = pack_whoami.header.uid_src;
                crc16_add2pack(p, pack_size);
                break;
            }
        }
    }
    uart_send_array(&uarts[0], p, pack_size);
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
    uid = uid_hash();
    pack_whoami.header.uid_src = uid;
    aura_recv_package(0);
}

void uart_recv_complete_callback(struct uart *u)
{
    uint32_t num = u->num;
		static uint32_t chunk_cnt[UART_COUNT] = {0};
		static uint32_t byte_cnt[UART_COUNT] = {0};
	
    enum state_recv *s = &states_recv[num];
    struct pack *p = &packs[num];
		uint32_t *cc = &chunk_cnt[num];
		uint32_t *bc = &byte_cnt[num];

    switch (*s) {
    case STATE_RECV_START: {
        if (p->header.data_sz) 
					*s = STATE_CHUNK_AMOUNT_CHECKED; //go to receiving crc
				else 
					*s = STATE_OVERALL_SIZE_CHECKED; // go to receiving chunks
				*cc = 0;
    } break;
		case STATE_OVERALL_SIZE_CHECKED:{
				*s = STATE_RECV_CHUNK_HDR;
				*bc += AURA_CHUNK_HDR_SIZE;
        uart_recv_array(u, &p->chunk[*cc], AURA_CHUNK_HDR_SIZE);
		} break;
    case STATE_RECV_CHUNK_HDR: {
        *s = STATE_RECV_CHUNK_DATA;
				*bc += p->chunk[*cc].size;
        uart_recv_array(u, p->chunk[*cc].data, p->chunk[*cc].size);
    } break;
		case STATE_RECV_CHUNK_DATA: {
				
        if(*bc < p->header.data_sz)
				{
					*cc = *cc+1;;
					*s = STATE_OVERALL_SIZE_CHECKED;
				}
				else
					*s = STATE_CHUNK_AMOUNT_CHECKED;

    } break;
		case STATE_CHUNK_AMOUNT_CHECKED: {
			*s = STATE_RECV_CRC;
			uint32_t pack_size = sizeof(struct header)
									 + *bc
									 + sizeof(crc16_t);
			if (crc16_is_valid(p, pack_size)) {
					aura_flags_pack_received[num] = 1;
			}
			aura_recv_package(num);
		} break;
    case STATE_RECV_CRC: {
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
