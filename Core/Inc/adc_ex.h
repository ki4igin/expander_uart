#include "stm32f4xx.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_cortex.h"

#define ADC_CHANNEL_SENS1	LL_ADC_CHANNEL_10
#define ADC_CHANNEL_SENS2	LL_ADC_CHANNEL_11
#define ADC_CHANNEL_SENS3	LL_ADC_CHANNEL_12
#define ADC_CHANNEL_SENS4	LL_ADC_CHANNEL_13
#define ADC_CHANNEL_SENS5	LL_ADC_CHANNEL_4
#define ADC_CHANNEL_SENS6	LL_ADC_CHANNEL_5
#define ADC_CHANNEL_SENS7	LL_ADC_CHANNEL_6
#define ADC_CHANNEL_SENS8	LL_ADC_CHANNEL_7

#define ADC_CHANNEL_UBAT	LL_ADC_CHANNEL_8

#define ADC_CH_CNT 9

extern uint16_t adc_buff[ADC_CH_CNT];

enum ADC_CH{
    ADC_CH_SENS1 = 0,
    ADC_CH_SENS2,
    ADC_CH_SENS3,
    ADC_CH_SENS4,
    ADC_CH_SENS5,
    ADC_CH_SENS6,
    ADC_CH_SENS7,
    ADC_CH_SENS8,
    ADC_CH_BAT
};

uint16_t adc_get_sens_state(uint16_t *src);
float adc_get_voltage(uint16_t *src);
	
