#include "aura.h"
#include "uid_hash.h"
#include "assert.h"
#include "crc16.h"
#include "usart_ex.h"
#include "fifo.h"

#define AURA_PROTOCOL      0x41525541U
#define AURA_PC_ID				 0x00000000U
#define AURA_MAX_REPEATERS 4
#define AURA_EXPANDER_ID   8
#define AURA_MAX_DATA_SIZE 128
#define AURA_MAX_DATA_SIZE_IN_CHUNK 16
#define AURA_MAX_CHUNK_CNT 18

#define AURA_CHUNK_HDR_SIZE 4
#define AURA_HDR_SIZE 20

struct fifo send_fifo;

enum state_recv {
    STATE_RECV_START = 0,
    STATE_RECV_CHUNK_HDR,
		STATE_RECV_CHUNK_DATA,
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
		CHUNK_TYPE_ARR_I8 = 10,
		CHUNK_TYPE_ARR_U8 = 11,
		CHUNK_TYPE_ARR_I16 = 12,
		CHUNK_TYPE_ARR_U16 = 13,
		CHUNK_TYPE_ARR_I32 = 14,
		CHUNK_TYPE_ARR_U32 = 15,
		CHUNK_TYPE_ARR_F32 = 16,
};

enum cmd {
    CMD_REQ_WHOAMI = 1,
		CMD_ANS_WHOAMI = 2,
    CMD_REQ_DATA = 3,
		CMD_ANS_DATA = 4,
};

enum chunk_id{
		CHUNK_ID_TYPE = 1,
		CHUNK_ID_UIDS = 2,
		CHUNK_ID_ONOFF  = 3,
		CHUNK_ID_TEMP = 4,
		CHUNK_ID_HUM = 5,
		CHUNK_ID_PRESS = 6,
		CHUNK_ID_WETSENS  = 7,
};

enum device_type{
		DEVICE_TYPE_LB75BD,
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
struct chunk_hdr{
		uint8_t id;
    uint8_t type;
    uint16_t size;
};
	
struct chunk {
    uint8_t id;
    uint8_t type;
    uint16_t size;
		uint8_t data[AURA_MAX_DATA_SIZE_IN_CHUNK];
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
		struct chunk_hdr chunk;
    uint32_t device_type;
    crc16_t crc;
} pack_whoami = {
    .header = {
        .protocol = AURA_PROTOCOL,
				.uid_dest = AURA_PC_ID,
				.cmd = CMD_ANS_WHOAMI,
				.data_sz = 8,
    },
    .chunk = {
        .id = CHUNK_ID_TYPE,
        .type = CHUNK_TYPE_U32,
        .size = 4,        
    },
		.device_type = DEVICE_TYPE_EXPANDER,
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
                           + p->header.data_sz
                           + sizeof(crc16_t);
        for (uint32_t i = 1; i < UART_COUNT; i++) {
            uart_send_array(&uarts[i], p, pack_size);
        }
    }
		
    switch (p->header.cmd) {
    case CMD_REQ_WHOAMI: {
        pack_whoami.header.cnt = p->header.cnt;
				struct pack_whoami *pnt = &pack_whoami;
        crc16_add2pack(pnt, 30);
        fifo_push(&send_fifo, pnt, 30);
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
                       + p->header.data_sz
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
		// taking data from fifo
		// takigng header
    struct pack *p = (struct pack *)fifo_get_ptail(&send_fifo);
		fifo_inc_tail(&send_fifo, sizeof(struct header));
		//taking chunks from fifo
		uint32_t data_sz = p->header.data_sz;
		uint32_t cnt = 0;
		while(data_sz)
		{
			struct chunk *c = (struct chunk *)fifo_get_ptail(&send_fifo);
			memcpy_u8(c,&p->chunk[cnt++], AURA_CHUNK_HDR_SIZE + c->size);
			data_sz -= AURA_CHUNK_HDR_SIZE + c->size;
			fifo_inc_tail(&send_fifo, AURA_CHUNK_HDR_SIZE + c->size);
		}
			
    uint32_t pack_size = sizeof(struct header)
                       + p->header.data_sz
                       + sizeof(crc16_t);
		
    if ((p->header.uid_src != pack_whoami.header.uid_src)
        && (p->header.cmd == CMD_ANS_WHOAMI)) {
				// check amounts of chunks
				uint32_t sens_id_sz = AURA_CHUNK_HDR_SIZE + p->chunk[0].size;
				if (p->header.data_sz > sens_id_sz)
				{
					//create new chunk
					struct chunk *nc = &p->chunk[1];
					nc->id = CHUNK_ID_UIDS;
					nc->type = CHUNK_TYPE_ARR_U32;
					nc->size = 4;
					memcpy_u8(&uid, nc->data, nc->size);
					
					p->header.data_sz += nc->size;
				}
				else
				{
					//add uid to second chunk
					uint32_t bias = p->header.data_sz - AURA_CHUNK_HDR_SIZE;
					memcpy_u8(&uid, (&p->chunk[1].data + bias), 4);
					
					p->header.data_sz += 4;
				}
				//calculate crc
				crc16_add2pack(p, pack_size);
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
				*cc = 0;
				*bc = 0;
        if (p->header.data_sz)
				{
					//receiving chunk hdr
					*s = STATE_RECV_CHUNK_HDR;
					*bc += AURA_CHUNK_HDR_SIZE;
					uart_recv_array(u, &p->chunk[*cc], AURA_CHUNK_HDR_SIZE);
				}
				else 
				{
					//receiving crc
					*s = STATE_RECV_CRC;
					uart_recv_array(u, &p->crc, 2);
				}

    } break;
    case STATE_RECV_CHUNK_HDR: {
        *s = STATE_RECV_CHUNK_DATA;
				*bc += p->chunk[*cc].size;
        uart_recv_array(u, p->chunk[*cc].data, p->chunk[*cc].size);
    } break;
		case STATE_RECV_CHUNK_DATA: {
				
        if(*bc < p->header.data_sz)
				{
					*s = STATE_RECV_CHUNK_HDR;
					*cc = *cc+1;;
					uart_recv_array(u, &p->chunk[*cc], AURA_CHUNK_HDR_SIZE);
				}
				else
				{
					*s = STATE_RECV_CRC;
					uart_recv_array(u, &p->crc, 2);
				}

    } break;
    case STATE_RECV_CRC: {

			uint32_t calc_crc = crc16_calc(p, AURA_HDR_SIZE);
			while(*cc)
			{
				uint32_t chunk_sz = AURA_CHUNK_HDR_SIZE+p->chunk[*cc].size;
				calc_crc = crc16_calc_continue(calc_crc, &p->chunk[*cc--], chunk_sz);
			}
			
			if (p->crc == calc_crc) {
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
