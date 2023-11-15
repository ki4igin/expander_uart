/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define MASTER_BAUD 115200
#define MASTER_DATAWIDTH LL_USART_DATAWIDTH_8B
#define MASTER_STOPBITS LL_USART_STOPBITS_1
#define MASTER_PARITY LL_USART_PARITY_NONE

#define SLAVE_BAUD 115200
#define SLAVE_DATAWIDTH LL_USART_DATAWIDTH_8B
#define SLAVE_STOPBITS LL_USART_STOPBITS_1
#define SLAVE_PARITY LL_USART_PARITY_NONE

#define DATA_SIZE 32+6+2*4+2

typedef struct usart_header
{
	uint32_t protocol;
	uint32_t cnt : 16;
	uint32_t dist : 8;
	uint32_t flags : 8;
	uint32_t src;
	uint32_t dest;
	uint32_t path[4];
} usart_header;

// should be sent before sending data
typedef struct __attribute__((packed)) usart_chunk_head
{
	uint32_t id: 16;
	uint32_t type: 16;
	uint16_t payload_sz;
} usart_chunk_head;

typedef struct __attribute__((packed)) usart_packet
{
	usart_header header;
	usart_chunk_head chunk_header;
	float data[2];
	uint16_t crc;
} usart_packet;
extern usart_packet usart_packets[8];

extern struct flags
{
	uint32_t usart1_tx_busy : 1;
	uint32_t usart1_tx_start : 1;
	uint32_t usart2_rx : 1;
	uint32_t usart3_rx : 1;
	uint32_t uart4_rx : 1;
	uint32_t uart5_rx : 1;
	uint32_t usart6_rx : 1;
	uint32_t uart7_rx : 1;
	uint32_t uart8_rx : 1;
	uint32_t uart9_rx : 1;
} flags;

enum usart_idx
{
	IDX_USART2,
	IDX_USART3,
	IDX_UART4,
	IDX_UART5,
	IDX_USART6,
	IDX_UART7,
	IDX_UART8,
	IDX_UART9
};

uint32_t usart_start_transmission(usart_packet usart_packets[8], struct flags *flags, uint32_t uid);
uint32_t usart_data_receiving(uint8_t *data, uint32_t idx, USART_TypeDef *USARTx);
void usart_txe_callback(const void *data);

/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_UART7_Init(void);
void MX_UART8_Init(void);
void MX_UART9_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

