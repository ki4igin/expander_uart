#ifndef __RELAY_H__
#define __RELAY_H__

#include "stm32f4xx_ll_gpio.h"
#include "gpio.h"

inline static uint32_t relay_is_open(uint16_t num)
{
    switch(num)
    {
        case RELAY_1:
            return LL_GPIO_IsOutputPinSet(GPIO_RELAY, GPIO_PIN_RELAY_1);
            break;
        case RELAY_2:
            return LL_GPIO_IsOutputPinSet(GPIO_RELAY, GPIO_PIN_RELAY_2);
            break;
    };
    return 3;
}

inline static void relay_open(uint16_t num)
{
    switch(num)
    {
        case RELAY_1:
            LL_GPIO_SetOutputPin(GPIO_RELAY, GPIO_PIN_RELAY_1);
            break;
        case RELAY_2:
            LL_GPIO_SetOutputPin(GPIO_RELAY, GPIO_PIN_RELAY_2);
            break;
    };
}

inline static void relay_close(uint16_t num)
{
    switch(num)
    {
        case RELAY_1:
            LL_GPIO_ResetOutputPin(GPIO_RELAY, GPIO_PIN_RELAY_1);
            break;
        case RELAY_2:
            LL_GPIO_ResetOutputPin(GPIO_RELAY, GPIO_PIN_RELAY_2);
            break;
    };
}

#endif
