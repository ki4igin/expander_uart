#include "stm32f4xx.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_cortex.h"

#define ADC_CHANNEL_SENS1	LL_ADC_CHANNEL10
#define ADC_CHANNEL_SENS2	LL_ADC_CHANNEL11
#define ADC_CHANNEL_SENS3	LL_ADC_CHANNEL12
#define ADC_CHANNEL_SENS4	LL_ADC_CHANNEL13
#define ADC_CHANNEL_SENS5	LL_ADC_CHANNEL4
#define ADC_CHANNEL_SENS6	LL_ADC_CHANNEL5
#define ADC_CHANNEL_SENS7	LL_ADC_CHANNEL6
#define ADC_CHANNEL_SENS8	LL_ADC_CHANNEL7

#define ADC_CHANNEL_UBAT	LL_ADC_CHANNEL8

void adc_meas(uint16_t *dest);
float adc_convert_to_voltage(uint16_t v);
	
