#include "usart_ex.h"
#include "gpio_ex.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

#define declare_usart(_name)                                         \
    [UART_NUM_##_name] = {                                           \
        .name = _name,                                               \
        .num = UART_NUM_##_name,                                     \
        .de = {.port = USART_RDE_GPIO_Port, .pin = _name##_RDE_Pin}, \
    }

struct uart uarts[UART_COUNT] = {
    declare_usart(USART1),
    declare_usart(USART2),
    declare_usart(USART3),
    declare_usart(UART4),
    declare_usart(UART5),
    declare_usart(USART6),
    declare_usart(UART7),
    declare_usart(UART8),
    declare_usart(UART9),
};

void uart_send_array(struct uart *u, void *data, uint32_t size)
{
    u->tx.data = data;
    u->tx.count = size;
    LL_GPIO_SetOutputPin(u->de.port, u->de.pin);
    LL_USART_EnableIT_TXE(u->name);
    LL_USART_EnableIT_TC(u->name);
}

void uart_recv_array(struct uart *u, void *data, uint32_t size)
{
    u->rx.data = data;
    u->rx.count = size;
    // 115200 / 1000 / (1 + 8 + 1) = 11.2 >~ 8
    u->timeout.ms = size / 8 + 2; 
    LL_USART_EnableIT_RXNE(u->name);
}

void uart_stop_recv(struct uart *u)
{
    LL_USART_DisableIT_RXNE(u->name);   
    u->timeout.is_enable = 0; 
}

void uart_irq_callback(struct uart *u)
{
    USART_TypeDef *name = u->name;
    if (LL_USART_IsEnabledIT_RXNE(name) && LL_USART_IsActiveFlag_RXNE(name)) {
        u->timeout.is_enable = 1;        
        *u->rx.data++ = LL_USART_ReceiveData8(name);
        u->rx.count--;
        if (u->rx.count == 0) {
            uart_stop_recv(u);
            uart_recv_complete_callback(u);
        }
    }
    if (LL_USART_IsEnabledIT_TXE(name) && LL_USART_IsActiveFlag_TXE(name)) {
        LL_USART_TransmitData8(name, *u->tx.data++);
        u->tx.count--;
        if (u->tx.count == 0) {
            LL_USART_DisableIT_TXE(name);
        }
    }
    if (LL_USART_IsEnabledIT_TC(name) && LL_USART_IsActiveFlag_TC(name)) {
        LL_USART_ClearFlag_TC(name);
        LL_USART_DisableIT_TC(name);
        LL_GPIO_ResetOutputPin(u->de.port, u->de.pin);
        uart_send_complete_callback(u);
    }
}

void tim6_update_callback()
{
    for (uint32_t i = 0; i < UART_COUNT; i++) {
        struct uart *u = &uarts[i];
        if (u->timeout.is_enable) {
           if (--u->timeout.ms == 0) {                
                uart_recv_timeout_callback(u);
            }
        }
    }
}

__WEAK void uart_send_complete_callback(struct uart *u)
{
    (void)u;
}

__WEAK void uart_recv_complete_callback(struct uart *u)
{
    (void)u;
}

__WEAK void uart_recv_timeout_callback(struct uart *u)
{
    (void)u;
}
