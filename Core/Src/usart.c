/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* UART4 init function */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**UART4 GPIO Configuration
  PA0   ------> UART4_TX
  PA1   ------> UART4_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* UART4 interrupt Init */
  NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART4_IRQn);

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART4, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART4);
  LL_USART_Enable(UART4);
  /* USER CODE BEGIN UART4_Init 2 */
	LL_USART_EnableIT_RXNE(UART4);
  /* USER CODE END UART4_Init 2 */

}
/* UART5 init function */
void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**UART5 GPIO Configuration
  PB5   ------> UART5_RX
  PB6   ------> UART5_TX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_11;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* UART5 interrupt Init */
  NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART5_IRQn);

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART5, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART5);
  LL_USART_Enable(UART5);
  /* USER CODE BEGIN UART5_Init 2 */
	LL_USART_EnableIT_RXNE(UART5);
  /* USER CODE END UART5_Init 2 */

}
/* UART7 init function */
void MX_UART7_Init(void)
{

  /* USER CODE BEGIN UART7_Init 0 */

  /* USER CODE END UART7_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART7);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  /**UART7 GPIO Configuration
  PE7   ------> UART7_RX
  PE8   ------> UART7_TX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7|LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* UART7 interrupt Init */
  NVIC_SetPriority(UART7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART7_IRQn);

  /* USER CODE BEGIN UART7_Init 1 */

  /* USER CODE END UART7_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART7, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART7);
  LL_USART_Enable(UART7);
  /* USER CODE BEGIN UART7_Init 2 */
	LL_USART_EnableIT_RXNE(UART7);
  /* USER CODE END UART7_Init 2 */

}
/* UART8 init function */
void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART8);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  /**UART8 GPIO Configuration
  PE0   ------> UART8_RX
  PE1   ------> UART8_TX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* UART8 interrupt Init */
  NVIC_SetPriority(UART8_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART8_IRQn);

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART8, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART8);
  LL_USART_Enable(UART8);
  /* USER CODE BEGIN UART8_Init 2 */
	LL_USART_EnableIT_RXNE(UART8);
  /* USER CODE END UART8_Init 2 */

}
/* UART9 init function */
void MX_UART9_Init(void)
{

  /* USER CODE BEGIN UART9_Init 0 */

  /* USER CODE END UART9_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_UART9);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  /**UART9 GPIO Configuration
  PD14   ------> UART9_RX
  PD15   ------> UART9_TX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_14|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_11;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* UART9 interrupt Init */
  NVIC_SetPriority(UART9_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART9_IRQn);

  /* USER CODE BEGIN UART9_Init 1 */

  /* USER CODE END UART9_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART9, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART9);
  LL_USART_Enable(UART9);
  /* USER CODE BEGIN UART9_Init 2 */
	LL_USART_EnableIT_RXNE(UART9);
  /* USER CODE END UART9_Init 2 */

}
/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */
	LL_USART_EnableIT_RXNE(USART1);
  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);
  /* USER CODE BEGIN USART2_Init 2 */
	LL_USART_EnableIT_RXNE(USART2);
  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  /**USART3 GPIO Configuration
  PB10   ------> USART3_TX
  PD9   ------> USART3_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USART3 interrupt Init */
  NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART3_IRQn);

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_Enable(USART3);
  /* USER CODE BEGIN USART3_Init 2 */
	LL_USART_EnableIT_RXNE(USART3);
  /* USER CODE END USART3_Init 2 */

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART6 GPIO Configuration
  PA11   ------> USART6_TX
  PA12   ------> USART6_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART6 interrupt Init */
  NVIC_SetPriority(USART6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART6_IRQn);

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART6, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART6);
  LL_USART_Enable(USART6);
  /* USER CODE BEGIN USART6_Init 2 */
	LL_USART_EnableIT_RXNE(USART6);
  /* USER CODE END USART6_Init 2 */

}

/* USER CODE BEGIN 1 */
static void usart_send (const void *s, uint32_t len, USART_TypeDef *USARTx)
{
	len--;
	do
	{
		while(!LL_USART_IsActiveFlag_TXE(USARTx));
		LL_USART_TransmitData8(USARTx, (*(uint8_t *)s++));
	} while(len--);
}

static void usart_receive (uint8_t *data, uint32_t len, USART_TypeDef *USARTx)
{
	do
	{
		*data++ = LL_USART_ReceiveData8(USARTx);
	} while (len--);
}

/** TXE FUNCTIONS **/
static uint32_t usart_sending (const void *data, uint32_t sz)
{
	static uint32_t cnt = 0;
	
	if (sz == 0)
		return 1;
	
	LL_USART_TransmitData8(USART1,*(uint8_t *)(data+cnt));
	cnt++;
	
	if (cnt > sz - 1) {
		cnt = 0;
		return 1;
	}
	
	return 0;
}

void usart_txe_callback(usart_packet *pack, uint16_t crc)
{
	static enum usart_send_state usart_send_state = STATE_SEND_HEADER;
	
	switch(usart_send_state)
	{
		case STATE_SEND_HEADER:
			usart_send_state += usart_sending(pack, HEADER_SIZE);
			break;
		case STATE_SEND_DATA:
			if(flags.whoami) usart_send_state += usart_sending(&pack->whoami, pack->hdr.chunk_header.payload_sz);
			else usart_send_state += usart_sending(pack->data, pack->hdr.chunk_header.payload_sz);
			break;
		case STATE_SEND_CRC:
			usart_send_state += usart_sending(&crc, 2);
			break;
		case STATE_END:
			LL_USART_DisableIT_TXE(USART1);
			LL_USART_EnableIT_TC(USART1);
			usart_send_state = 	STATE_SEND_HEADER;
			break;
	}
}
static void usart_changing_pack(usart_packet *pack, uint16_t *crc, uint32_t uid)
{
	//TODO
	//pack->whoami.path[]
	*crc = crc16(0xFFFF, pack, HEADER_SIZE); //all except crc
	*crc = crc16(*crc, pack->data, pack->hdr.chunk_header.payload_sz);
}

static uint32_t usart_flags_rx_processing (struct flags *flags)
{
	uint32_t idx = 100;
	
	if (flags->usart2_rx)
	{
		idx = IDX_USART2;
		flags->usart2_rx = 0;
	}
	else if (flags->usart3_rx)
	{
		idx = IDX_USART3;
		flags->usart3_rx = 0;
	}
	else if (flags->uart4_rx)
	{
		idx = IDX_UART4;
		flags->uart4_rx = 0;
	}
	else if (flags->uart5_rx)
	{
		idx = IDX_UART5;
		flags->uart5_rx = 0;
	}
	else if (flags->usart6_rx)
	{
		idx = IDX_USART6;
		flags->usart6_rx = 0;
	}
	else if (flags->uart7_rx)
	{
		idx = IDX_UART7;
		flags->uart7_rx = 0;
	}
	else if (flags->uart8_rx)
	{
		idx = IDX_UART8;
		flags->uart8_rx = 0;
	}
	else if (flags->uart9_rx)
	{
		idx = IDX_UART9;
		flags->uart9_rx = 0;
	}
	return idx;
}

uint32_t usart_start_transmission(usart_packet usart_packets[8], uint16_t crc[8], struct flags *flags, uint32_t uid)
{
	uint32_t idx = usart_flags_rx_processing(flags);
	if (idx < IDX_UART9+1)
	{
		usart_changing_pack(&usart_packets[idx], &crc[idx], uid);
		LL_GPIO_SetOutputPin(GPIO_RDE, USART1_RDE_PIN);
		usart_txe_callback(&usart_packets[idx], crc[idx]);
	}
	return idx;
}

/** RXNE FUNCTIONS **/
static uint32_t usart_receiving(uint8_t *data, uint32_t idx, struct flags *flags, USART_TypeDef *USARTx, uint32_t sz)
{
	static uint32_t counter[8] = {0};
	
	if (sz == 0) 
		return 1;
	
	if(LL_USART_IsActiveFlag_RXNE(USARTx) && LL_USART_IsEnabledIT_RXNE(USARTx))
  {
		*(data + counter[idx]) = LL_USART_ReceiveData8(USARTx);
		counter[idx]++;
		
		if (counter[idx] > sz - 1)
		{
			counter[idx] = 0;
			return 1;
		}
	}
	return 0;
}

uint32_t usart_rxne_callback(usart_packet usart_packets[8], uint16_t crc[8], uint32_t idx, struct flags *flags, USART_TypeDef *USARTx)
{
	static enum usart_rcv_state usart_rcv_state[8] = {STATE_RCV_HEADER};
	
	switch (usart_rcv_state[idx]){
		case STATE_RCV_HEADER:
			usart_rcv_state[idx] += usart_receiving((uint8_t *)&usart_packets[idx], idx, flags, USARTx, HEADER_SIZE);
			break;
		case STATE_RCV_DATA:
			usart_rcv_state[idx] += usart_receiving((uint8_t *)&usart_packets[idx].data, idx, flags, USARTx, usart_packets[idx].hdr.chunk_header.payload_sz);
			break;
		case STATE_RCV_CRC:
			usart_rcv_state[idx] -= 2*usart_receiving((uint8_t *)&crc[idx], idx, flags, USARTx, 2);
			if (usart_rcv_state[idx] == STATE_RCV_HEADER && crc[idx] == crc16(0xFFFF, &usart_packets[idx],HEADER_SIZE+usart_packets[idx].hdr.chunk_header.payload_sz))
			{
				flags->usart1_tx_start = 1;
				return 1;
			}
			break;
	}
	
	return 0;
}

static uint32_t check_crc(usart_packet *pack, uint16_t crc, uint32_t whoami)
{
	uint16_t checking_crc =  crc16(0xFFFF, pack ,HEADER_SIZE);
	if (whoami) checking_crc = crc16(checking_crc,&pack->whoami,pack->hdr.chunk_header.payload_sz);
	else checking_crc = crc16(checking_crc, &pack->data, pack->hdr.chunk_header.payload_sz);
	return (crc == checking_crc);
}


static void rxne_check_whoami (usart_packet *pack, struct flags *flags)
{
	flags->whoami = (pack->hdr.chunk_header.id == 0);
}

uint32_t usart1_rxne_callback(usart_packet *usart_packet, uint16_t crc, struct flags *flags)
{
	static enum usart_rcv_state usart_rcv_state = {STATE_RCV_HEADER};
	
	switch (usart_rcv_state){
		case STATE_RCV_HEADER:
			usart_rcv_state += usart_receiving((uint8_t *)usart_packets, idx, flags, USART1, HEADER_SIZE);
			break;
		case STATE_RCV_DATA:
			usart_rcv_state += usart_receiving((uint8_t *)usart_packet->data, idx, flags, USART1, usart_packets[idx].hdr.chunk_header.payload_sz);
			break;
		case STATE_RCV_CRC:
			usart_rcv_state -= 2*usart_receiving((uint8_t *)&crc, idx, flags, USART1, 2);
			if (usart_rcv_state == STATE_RCV_HEADER)
			{
				flags->whoami = (usart_packet->hdr.chunk_header.type == 0)&&(usart_packet->hdr.chunk_header.payload_sz == 0);
				flags->usart1_tx_start = check_crc(usart_packet, crc, flags->whoami);
				return flags->usart1_tx_start;
			}
			break;
	}
	
	return 0;
}


/* USER CODE END 1 */
