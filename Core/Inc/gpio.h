#ifndef __GPIO_H__
#define __GPIO_H__

#include "main.h"

void MX_GPIO_Init(void);

#define GPIO_LED GPIOD
#define GPIO_PIN_LED_GREEN LL_GPIO_PIN_4
#define GPIO_PIN_LED_RED LL_GPIO_PIN_5

#define GPIO_RELAY GPIOD
#define GPIO_PIN_RELAY_1 LL_GPIO_PIN_0
#define GPIO_PIN_RELAY_2 LL_GPIO_PIN_1

enum relay
{
    RELAY_1 = 1,
    RELAY_2
};
#endif /*__ GPIO_H__ */
