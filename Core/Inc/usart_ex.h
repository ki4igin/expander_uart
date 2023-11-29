#ifndef __USART_EX_H__
#define __USART_EX_H__

#include "stm32f4xx.h"
#include "gpio_ex.h"

#define UART_COUNT 9

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_UART7_Init(void);
void MX_UART8_Init(void);
void MX_UART9_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

enum uart_num {
    UART_NUM_USART1 = 0,
    UART_NUM_USART2,
    UART_NUM_USART3,
    UART_NUM_UART4,
    UART_NUM_UART5,
    UART_NUM_USART6,
    UART_NUM_UART7,
    UART_NUM_UART8,
    UART_NUM_UART9,
};

struct uart {
    uint32_t num;

    struct {
        uint16_t is_enable;
        uint16_t ms;
    } timeout;

    USART_TypeDef *name;

    struct {
        uint32_t count;
        uint8_t *data;
    } rx, tx;

    struct gpio de;
};

extern struct uart uarts[UART_COUNT];

void uart_send_array(struct uart *u, void *data, uint32_t size);
void uart_recv_array(struct uart *u, void *data, uint32_t size);
void uart_stop_recv(struct uart *u);

void uart_irq_callback(struct uart *u);
void tim6_update_callback();

void uart_send_complete_callback(struct uart *u);
void uart_recv_complete_callback(struct uart *u);
void uart_recv_timeout_callback(struct uart *u);

#endif /* __USART_H__ */
