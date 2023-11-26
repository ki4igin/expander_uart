#ifndef __RELAY_H__
#define __RELAY_H__

#include "stm32f4xx_ll_gpio.h"
#include "gpio_ex.h"

enum relay {
    RELAY1 = 1,
    RELAY2 = 2,
};

const struct gpio relay_gpio[] = {
    [RELAY1] = {.port = RELAY_GPIO_Port, .pin = RELAY1_Pin},
    [RELAY2] = {.port = RELAY_GPIO_Port, .pin = RELAY2_Pin},
};

inline static uint32_t relay_is_open(enum relay r)
{
    struct gpio g = relay_gpio[r];
    return LL_GPIO_IsOutputPinSet(g.port, g.pin);
}

inline static void relay_open(enum relay r)
{
    struct gpio g = relay_gpio[r];
    return LL_GPIO_SetOutputPin(g.port, g.pin);
}

inline static void relay_close(enum relay r)
{
    struct gpio g = relay_gpio[r];
    return LL_GPIO_ResetOutputPin(g.port, g.pin);
}

#endif
