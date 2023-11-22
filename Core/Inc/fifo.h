#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>
#include "tools.h"

#define FIFO_SIZE (128 * 64)
#define FIFO_LEN  (FIFO_SIZE / 4)

#define USE_U8_DATA
// #define USE_U32_DATA

#ifdef USE_U8_DATA

struct fifo {
    uint32_t last_len_jump;
    uint32_t head;
    uint32_t tail;
    uint8_t data[FIFO_LEN];
};

inline static void fifo_inc_head(struct fifo *f, uint32_t size)
{
    f->head += size;
    f->head &= FIFO_LEN - 1;
}

inline static void fifo_inc_tail(struct fifo *f, uint32_t size)
{
    f->tail += size;
    f->tail &= FIFO_LEN - 1;
}

inline static uint8_t *fifo_get_ptail(struct fifo *f)
{
    if ((FIFO_LEN - f->tail) < f->last_len_jump) {
        f->tail = 0;
    }
    return &f->data[f->tail];
}

inline static uint8_t *fifo_get_phead(struct fifo *f, uint32_t size)
{
    if ((FIFO_LEN - f->head) < size) {
        f->last_len_jump = size;
        f->head = 0;
    }

    return &f->data[f->head];
}

inline static void fifo_push(struct fifo *f, void *data, uint32_t size)
{
    uint8_t *p = fifo_get_phead(f, size);
    memcpy_u8(data, p, size);
    fifo_inc_head(f, size);
}

inline static uint32_t fifo_is_empty(struct fifo *f)
{
    return f->head == f->tail;
}

inline static uint32_t fifo_is_not_empty(struct fifo *f)
{
    return f->head != f->tail;
}

inline static uint32_t fifo_is_full(struct fifo *f)
{
    return ((f->head + 1) & (FIFO_LEN - 1)) == f->tail;
}

#endif

#ifdef USE_U32_DATA
struct pack_ptrs {
    uint32_t *data;
    uint32_t size;
};

struct fifo {
    uint32_t last_len_jump;
    uint32_t head;
    uint32_t tail;
    uint32_t data[FIFO_LEN];
};

inline static void fifo_inc_head(struct fifo *f, uint32_t len)
{
    f->head += len;
    f->head &= FIFO_LEN - 1;
}

inline static void fifo_inc_tail(struct fifo *f, uint32_t size)
{
    uint32_t len = (size + 3) >> 2; // len в количестве u32
    f->tail += len;
    f->tail &= FIFO_LEN - 1;
}

inline static uint32_t *fifo_get_ptail(struct fifo *f)
{
    if ((FIFO_LEN - f->tail) < f->last_len_jump) {
        f->tail = 0;
    }
    return &f->data[f->tail];
}

inline static uint32_t *fifo_get_phead(struct fifo *f, uint32_t len)
{
    if ((FIFO_LEN - f->head) < len) {
        f->last_len_jump = len;
        f->head = 0;
    }

    return &f->data[f->head];
}

//inline 
static void fifo_push(struct fifo *f, void *data, uint32_t size)
{
    uint32_t len = (size + 3) >> 2; // len в количестве u32
    uint32_t *p = fifo_get_phead(f, len);
    memcpy_u32(p, data, len << 2);
    fifo_inc_head(f, len);
}

inline static uint32_t fifo_is_empty(struct fifo *f)
{
    return f->head == f->tail;
}

inline static uint32_t fifo_is_not_empty(struct fifo *f)
{
    return f->head != f->tail;
}

inline static uint32_t fifo_is_full(struct fifo *f)
{
    return ((f->head + 1) & (FIFO_LEN - 1)) == f->tail;
}

#endif

#endif