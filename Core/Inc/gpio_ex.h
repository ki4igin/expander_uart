
#ifndef __GPIO_EX_H__
#define __GPIO_EX_H__

#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_utils.h"

#define USART_RDE_GPIO_Port GPIOC
#define USART1_RDE_Pin      LL_GPIO_PIN_4
#define USART2_RDE_Pin      LL_GPIO_PIN_5
#define USART3_RDE_Pin      LL_GPIO_PIN_6
#define UART4_RDE_Pin       LL_GPIO_PIN_7
#define UART5_RDE_Pin       LL_GPIO_PIN_8
#define USART6_RDE_Pin      LL_GPIO_PIN_9
#define UART7_RDE_Pin       LL_GPIO_PIN_10
#define UART8_RDE_Pin       LL_GPIO_PIN_11
#define UART9_RDE_Pin       LL_GPIO_PIN_12

#define RELAY_GPIO_Port     GPIOD
#define RELAY1_Pin          LL_GPIO_PIN_0
#define RELAY2_Pin          LL_GPIO_PIN_1

#define LED_GPIO_Port       GPIOD
#define GLED_GREEN_Pin      LL_GPIO_PIN_4
#define GLED_RED_Pin        LL_GPIO_PIN_5

#define gpio_on_off_toggle_declare(_name, _port, _pin) \
    inline static void gpio_##_name##_on(void)         \
    {                                                  \
        LL_GPIO_SetOutputPin(_port, _pin);             \
    }                                                  \
    inline static void gpio_##_name##_off(void)        \
    {                                                  \
        LL_GPIO_ResetOutputPin(_port, _pin);           \
    }                                                  \
    inline static void gpio_##_name##_toggle(void)     \
    {                                                  \
        LL_GPIO_TogglePin(_port, _pin);                \
    }

// clang-format off
gpio_on_off_toggle_declare(ledr, LED_GPIO_Port, GLED_RED_Pin)
gpio_on_off_toggle_declare(ledg, LED_GPIO_Port, GLED_GREEN_Pin)
// clang-format on

static inline void led_blink_red()
{
	gpio_ledg_toggle();
	LL_mDelay(500);
	gpio_ledg_toggle();
	LL_mDelay(500);
}

struct gpio {
    GPIO_TypeDef *port;
    uint32_t pin;
};

#endif