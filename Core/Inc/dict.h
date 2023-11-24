#ifndef __DICT_H__
#define __DICT_H__

#include "stm32f4xx.h"
#include "tools.h"

#define sizeof_u32(_val) (sizeof(_val) / sizeof(uint32_t))

#define dict_declare(_name, _count)          \
    uint32_t _name##_buf[sizeof(struct dict) \
                         + sizeof(struct keyvalue) * (_count)] = {0}

struct keyvalue {
    uint32_t key;
    uint32_t value;
};

struct dict {
    uint32_t count;
    struct keyvalue kvs[];
};

inline static void dict_clear(struct dict *d)
{
    if (d->count == 0) {
        return;
    }
    arr_clear_u32(d->kvs, d->count * sizeof_u32(d->kvs[0]));
    d->count = 0;
}

inline static uint32_t dict_get_idx(struct dict *p, uint32_t key)
{
    for (uint32_t i = 0; i < p->count; i++) {
        if (p->kvs[i].key == key) {
            return i;
        }
    }
    return -1U;
}

inline static void dict_add(struct dict *p, uint32_t key, uint32_t value)
{
    p->kvs[p->count++] = (struct keyvalue){.key = key, .value = value};
}

#endif
