#ifndef __STACK_H__
#define __STACK_H__

#include "stdint.h"

#define stack_declare(_name, _count)            \
     uint8_t _name##_stack[sizeof(struct stack) \
                         + sizeof(uint8_t) * (_count)] __ALIGNED(4) = {0}

struct stack {
    uint32_t count;
    uint8_t data[];
};

static void stack_push(struct stack *s, uint8_t data)
{
    s->data[s->count++] = data;
}

static uint8_t stack_pop(struct stack *s)
{
    return s->data[--s->count];
}

static uint8_t stack_get_count(struct stack *s)
{
    return s->count;
}

static void stack_clear(struct stack *s)
{
    s->count = 0;
}

static uint32_t stack_is_empty(struct stack *s)
{
    return s->count == 0;
}

#endif
