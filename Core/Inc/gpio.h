#ifndef __GPIO_H__
#define __GPIO_H__

#include "main.h"

void MX_GPIO_Init(void);

#define GPIO_LED GPIOD
#define GPIO_PIN_LED_GREEN LL_GPIO_PIN_4
#define GPIO_PIN_LED_RED LL_GPIO_PIN_5
#endif /*__ GPIO_H__ */
