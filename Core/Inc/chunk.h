#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "stdint.h"

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

enum chunk_id {
    CHUNK_ID_TYPE = 1,
    CHUNK_ID_UIDS = 2,
    CHUNK_ID_WETSENS = 3,
    CHUNK_ID_RELAY1_STATUS = 4,
    CHUNK_ID_RELAY2_STATUS = 5,
    CHUNK_ID_BAT_VOLT = 6,
};

struct chunk_hdr {
    uint8_t id;
    uint8_t type;
    uint16_t size;
};

struct chunk {
    struct chunk_hdr hdr;
    uint8_t data[];
};

struct chunk_u16 {
    struct chunk_hdr hdr;
    uint16_t val;
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

static void add_chunk_u16(void **next_chunk, enum chunk_id id, uint16_t val)
{
    struct chunk_u16 *c = (struct chunk_u16 *)*next_chunk;
    c->hdr.id = id;
    c->hdr.type = CHUNK_TYPE_U16;
    c->hdr.size = sizeof(c->val);
    c->val = val;
    *next_chunk = (void *)((uint32_t)*next_chunk + sizeof(*c));
}

static void add_chunk_u32(void **next_chunk, enum chunk_id id, uint32_t val)
{
    struct chunk_u32 *c = (struct chunk_u32 *)*next_chunk;
    c->hdr.id = id;
    c->hdr.type = CHUNK_TYPE_U32;
    c->hdr.size = sizeof(c->val);
    c->val = val;
    *next_chunk = (void *)((uint32_t)*next_chunk + sizeof(*c));
}

#endif
