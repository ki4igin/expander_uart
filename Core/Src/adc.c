/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
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
#include "adc.h"
#include "adc_ex.h"

/* USER CODE BEGIN 0 */

struct rank_ch {
    uint32_t rank;
    uint32_t ch;
};

// clang-formant off
const struct rank_ch reg_chs[ADC_CH_COUNT] = {
    [ADC_CH_SENS1] = {.rank = LL_ADC_REG_RANK_1, .ch = LL_ADC_CHANNEL_10},
    [ADC_CH_SENS2] = {.rank = LL_ADC_REG_RANK_2, .ch = LL_ADC_CHANNEL_11},
    [ADC_CH_SENS3] = {.rank = LL_ADC_REG_RANK_3, .ch = LL_ADC_CHANNEL_12},
    [ADC_CH_SENS4] = {.rank = LL_ADC_REG_RANK_4, .ch = LL_ADC_CHANNEL_13},
    [ADC_CH_SENS5] = {.rank = LL_ADC_REG_RANK_5,  .ch = LL_ADC_CHANNEL_4},
    [ADC_CH_SENS6] = {.rank = LL_ADC_REG_RANK_6,  .ch = LL_ADC_CHANNEL_5},
    [ADC_CH_SENS7] = {.rank = LL_ADC_REG_RANK_7,  .ch = LL_ADC_CHANNEL_6},
    [ADC_CH_SENS8] = {.rank = LL_ADC_REG_RANK_8,  .ch = LL_ADC_CHANNEL_7},
    [ADC_CH_BAT] = {.rank = LL_ADC_REG_RANK_9,  .ch = LL_ADC_CHANNEL_8},
};
// clang-formant on

uint16_t adc_buf[ADC_CH_COUNT];

/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init(void)
{
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    /**ADC1 GPIO Configuration
    PC0   ------> ADC1_IN10
    PC1   ------> ADC1_IN11
    PC2   ------> ADC1_IN12
    PC3   ------> ADC1_IN13
    PA4   ------> ADC1_IN4
    PA5   ------> ADC1_IN5
    PA6   ------> ADC1_IN6
    PA7   ------> ADC1_IN7
    PB0   ------> ADC1_IN8
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    NVIC_SetPriority(ADC_IRQn, 0); /* ADC IRQ greater priority than DMA IRQ */
    NVIC_EnableIRQ(ADC_IRQn);

    /** Common config
     */
    ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
    LL_ADC_Init(ADC1, &ADC_InitStruct);
    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_9RANKS;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
    // LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_UNITARY_CONV);
    ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
    /** Configure Regular Channel
     */
    for (uint32_t i = 0; i < ADC_CH_COUNT; i++) {
        LL_ADC_REG_SetSequencerRanks(ADC1, reg_chs[i].rank, reg_chs[i].ch);
        LL_ADC_SetChannelSamplingTime(ADC1, reg_chs[i].ch, LL_ADC_SAMPLINGTIME_480CYCLES);
    }

    LL_ADC_EnableIT_OVR(ADC1);

    LL_ADC_Enable(ADC1);
}

/* USER CODE BEGIN 1 */
/**
 * @brief  This function configures DMA for transfer of data from ADC
 * @param  None
 * @retval None
 */
void ADC_Configure_DMA(void)
{
    /*## Configuration of NVIC #################################################*/
    /* Configure NVIC to enable DMA interruptions */
    NVIC_SetPriority(DMA2_Stream0_IRQn, 1); /* DMA IRQ lower priority than ADC IRQ */
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);

    /*## Configuration of DMA ##################################################*/
    /* Enable the peripheral clock of DMA */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

    /* Configure the DMA transfer */
    /*  - DMA transfer in circular mode to match with ADC configuration:        */
    /*    DMA unlimited requests.                                               */
    /*  - DMA transfer from ADC without address increment.                      */
    /*  - DMA transfer to memory with address increment.                        */
    /*  - DMA transfer from ADC by half-word to match with ADC configuration:   */
    /*    ADC resolution 12 bits.                                               */
    /*  - DMA transfer to memory by half-word to match with ADC conversion data */
    /*    buffer variable type: half-word.                                      */
    LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_0, LL_DMA_CHANNEL_0);
    LL_DMA_ConfigTransfer(DMA2, LL_DMA_STREAM_0,
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY
                              | LL_DMA_MODE_CIRCULAR
                              | LL_DMA_PERIPH_NOINCREMENT
                              | LL_DMA_MEMORY_INCREMENT
                              | LL_DMA_PDATAALIGN_HALFWORD
                              | LL_DMA_MDATAALIGN_HALFWORD
                              | LL_DMA_PRIORITY_HIGH);

    /* Set DMA transfer addresses of source and destination */
    LL_DMA_ConfigAddresses(DMA2,
                           LL_DMA_STREAM_0,
                           LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                           (uint32_t)adc_buf,
                           LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    /* Set DMA transfer size */
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, ADC_CH_COUNT);

    /* Enable DMA transfer interruption: transfer complete */
    LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);

    /* Enable DMA transfer interruption: transfer error */
    LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_0);

    /*## Activation of DMA #####################################################*/
    /* Enable the DMA transfer */
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
}



/* USER CODE END 1 */
