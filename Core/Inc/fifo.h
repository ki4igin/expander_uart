#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdint.h>

#define fifo_declare(_name, _count)          \
    uint32_t _name##_buf[sizeof(struct fifo) \
                         + sizeof(uint32_t) * (_count)] = {[0] = (_count)-1}

struct fifo {
    const uint32_t mask;
    uint32_t head;
    uint32_t tail;
    uint32_t data[];
};

#define fifo_inc_mask(_id, _val) \
    ((_id)->_val = ((_id)->_val + 1) & (_id->mask))

inline static void fifo_push(struct fifo *id, uint32_t data)
{
    id->data[id->head] = data;
    fifo_inc_mask(id, head);
}

inline static uint32_t fifo_pop(struct fifo *id)
{
    uint32_t data = id->data[id->tail];
    fifo_inc_mask(id, tail);
    return data;
}

inline static uint32_t fifo_is_empty(struct fifo *id)
{
    return id->head == id->tail;
}

inline static uint32_t fifo_is_nonempty(struct fifo *id)
{
    return id->head != id->tail;
}

inline static uint32_t fifo_is_full(struct fifo *id)
{
    return ((id->head + 1) & id->mask) == id->tail;
}

#undef fifo_inc_mask

#endif