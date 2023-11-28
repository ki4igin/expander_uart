#ifndef __ADC_EX_H__
#define __ADC_EX_H__

#include "stm32f4xx.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_cortex.h"

enum ADC_CH {
    ADC_CH_SENS1 = 0,
    ADC_CH_SENS2,
    ADC_CH_SENS3,
    ADC_CH_SENS4,
    ADC_CH_SENS5,
    ADC_CH_SENS6,
    ADC_CH_SENS7,
    ADC_CH_SENS8,
    ADC_CH_BAT,
    ADC_CH_COUNT,
    ADC_CH_SENS_COUNT = 8,
};

uint16_t adc_get_sens_state(uint16_t *src);
float adc_get_voltage(uint16_t *src);

void adc_dma_irq_callback(void);

void adc_bat_data_ready_callback(uint16_t data);
void adc_sens_data_ready_callback(uint32_t sens_count,
                         uint16_t data[static sens_count]);

#endif
