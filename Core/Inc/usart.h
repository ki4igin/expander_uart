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
#define HEADER_SIZE 16+4
#define DATA_MAX_SIZE 2

#define PC_ID 0x00000000
#define PROTOCOL_AURA 0x41525541
#define ID_FCN_EXPANDER 8

#define GPIO_RDE GPIOC
#define USART1_RDE_PIN LL_GPIO_PIN_4

#define USART2_RDE_PIN LL_GPIO_PIN_5
#define USART3_RDE_PIN LL_GPIO_PIN_6
#define UART4_RDE_PIN LL_GPIO_PIN_7
#define UART5_RDE_PIN LL_GPIO_PIN_8
#define USART6_RDE_PIN LL_GPIO_PIN_9
#define UART7_RDE_PIN LL_GPIO_PIN_10
#define UART8_RDE_PIN LL_GPIO_PIN_11
#define UART9_RDE_PIN LL_GPIO_PIN_12

typedef struct usart_header
{
	uint32_t protocol;
	uint32_t cnt;
	uint32_t src;
	uint32_t dest;
} usart_header;

enum data_type
{
	DATA_TYPE_CHAR = 1,
	DATA_TYPE_UCHAR,
	DATA_TYPE_SHORT,
	DATA_TYPE_USHORT,
	DATA_TYPE_INT,
	DATA_TYPE_UINT,
	DATA_TYPE_FLOAT,
	DATA_TYPE_DOUBLE,
	DATA_TYPE_STRING
};
enum fcn_id
{
	FCN_ID_WHOAMI = 0,
	FCN_ID_DATA
};

// should be sent before sending data
typedef struct
{
	uint32_t id: 8;
	uint32_t type: 8;
	uint32_t payload_sz : 16;
} usart_chunk_head;

typedef struct
{
	usart_header header;
	usart_chunk_head chunk_header;
} usart_data_header;

typedef struct 
{
	uint32_t sensor_type;
	uint32_t path[4];
} whoami_pack;

typedef struct
{
	usart_data_header hdr;
	whoami_pack whoami;
	uint8_t data[];
} usart_packet;

extern usart_packet usart_packets[8];
extern uint16_t crc[8];

extern usart_packet usart1_packet;
extern uint16_t usart1_crc;

extern struct flags
{
	uint32_t usart1_tx_busy : 1;
	uint32_t usart1_tx_start : 1;
	uint32_t usart1_rx_end : 1;
	uint32_t usart2_rx : 1;
	uint32_t usart3_rx : 1;
	uint32_t uart4_rx : 1;
	uint32_t uart5_rx : 1;
	uint32_t usart6_rx : 1;
	uint32_t uart7_rx : 1;
	uint32_t uart8_rx : 1;
	uint32_t uart9_rx : 1;
	uint32_t whoami : 1;
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

enum usart_rcv_state
{
	STATE_RCV_HEADER = 0,
	STATE_RCV_DATA,
	STATE_RCV_CRC,
};

enum usart_send_state
{
	STATE_SEND_HEADER = 0,
	STATE_SEND_DATA,
	STATE_SEND_CRC,
	STATE_END,
};

uint32_t usart_start_transmission(usart_packet usart_packets[8], uint16_t crc[8], struct flags *flags, uint32_t uid);
uint32_t usart_rxne_callback(usart_packet usart_packets[8], uint16_t crc[8], uint32_t idx, struct flags *flags, USART_TypeDef *USARTx);

uint32_t usart1_rxne_callback(usart_packet *usart_packet, uint16_t crc, struct flags *flags);
void usart_txe_callback(usart_packet *pack, uint16_t crc, USART_TypeDef USARTx);

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

