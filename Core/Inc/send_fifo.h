#ifndef __SEND_FIFO_H
#define __SEND_FIFO_H

#include <stdint.h>
#include "tools.h"

#define SEND_FIFO_SIZE (128 * 64)
#define SEND_FIFO_LEN  (SEND_FIFO_SIZE / 4)

#define USE_U8_DATA
// #define USE_U32_DATA

#ifdef USE_U8_DATA

struct send_fifo {
    uint32_t last_len_jump;
    uint32_t head;
    uint32_t tail;
    uint8_t data[SEND_FIFO_LEN];
};

inline static void send_fifo_inc_head(struct send_fifo *f, uint32_t size)
{
    f->head += size;
    f->head &= SEND_FIFO_LEN - 1;
}

inline static void send_fifo_inc_tail(struct send_fifo *f, uint32_t size)
{
    f->tail += size;
    f->tail &= SEND_FIFO_LEN - 1;
}

inline static uint8_t *send_fifo_get_ptail(struct send_fifo *f)
{
    if ((SEND_FIFO_LEN - f->tail) < f->last_len_jump) {
        f->tail = 0;
    }
    return &f->data[f->tail];
}

inline static uint8_t *send_fifo_get_phead(struct send_fifo *f, uint32_t size)
{
    if ((SEND_FIFO_LEN - f->head) < size) {
        f->last_len_jump = size;
        f->head = 0;
    }

    return &f->data[f->head];
}

inline static void send_fifo_push(struct send_fifo *f, void *data, uint32_t size)
{
    uint8_t *p = send_fifo_get_phead(f, size);
    memcpy_u8(data, p, size);
    send_fifo_inc_head(f, size);
}

inline static uint32_t send_fifo_is_empty(struct send_fifo *f)
{
    return f->head == f->tail;
}

inline static uint32_t send_fifo_is_not_empty(struct send_fifo *f)
{
    return f->head != f->tail;
}

inline static uint32_t send_fifo_is_full(struct send_fifo *f)
{
    return ((f->head + 1) & (SEND_FIFO_LEN - 1)) == f->tail;
}

#endif

#ifdef USE_U32_DATA
struct pack_ptrs {
    uint32_t *data;
    uint32_t size;
};

struct send_fifo {
    uint32_t last_len_jump;
    uint32_t head;
    uint32_t tail;
    uint32_t data[SEND_FIFO_LEN];
};

inline static void send_fifo_inc_head(struct send_fifo *f, uint32_t len)
{
    f->head += len;
    f->head &= SEND_FIFO_LEN - 1;
}

inline static void send_fifo_inc_tail(struct send_fifo *f, uint32_t size)
{
    uint32_t len = (size + 3) >> 2; // len в количестве u32
    f->tail += len;
    f->tail &= SEND_FIFO_LEN - 1;
}

inline static uint32_t *send_fifo_get_ptail(struct send_fifo *f)
{
    if ((SEND_FIFO_LEN - f->tail) < f->last_len_jump) {
        f->tail = 0;
    }
    return &f->data[f->tail];
}

inline static uint32_t *send_fifo_get_phead(struct send_fifo *f, uint32_t len)
{
    if ((SEND_FIFO_LEN - f->head) < len) {
        f->last_len_jump = len;
        f->head = 0;
    }

    return &f->data[f->head];
}

//inline 
static void send_fifo_push(struct send_fifo *f, void *data, uint32_t size)
{
    uint32_t len = (size + 3) >> 2; // len в количестве u32
    uint32_t *p = send_fifo_get_phead(f, len);
    memcpy_u32(p, data, len << 2);
    send_fifo_inc_head(f, len);
}

inline static uint32_t send_fifo_is_empty(struct send_fifo *f)
{
    return f->head == f->tail;
}

inline static uint32_t send_fifo_is_not_empty(struct send_fifo *f)
{
    return f->head != f->tail;
}

inline static uint32_t send_fifo_is_full(struct send_fifo *f)
{
    return ((f->head + 1) & (SEND_FIFO_LEN - 1)) == f->tail;
}

#endif

#endif