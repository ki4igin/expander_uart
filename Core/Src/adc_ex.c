#include "adc_ex.h"

#define ADC_CH_CNT 9
#define BATTERY_MAX_VOLTAGE 17.0f

static uint32_t queue[9] = {5,6,7,8,0,1,2,3,4};

void adc_meas(uint16_t *dest)
{
	LL_ADC_REG_StartConversionSWStart(ADC1);
	for(uint32_t i = 0; i < ADC_CH_CNT; i++)
	{
		while(!LL_ADC_IsActiveFlag_EOCS(ADC1));
		//LL_ADC_ClearFlag_EOCS(ADC1);
		*(dest + queue[i]) = LL_ADC_REG_ReadConversionData8(ADC1);
	}
}

float adc_convert_to_voltage(uint16_t v)
{
	return v*BATTERY_MAX_VOLTAGE/4095.0f;
}
