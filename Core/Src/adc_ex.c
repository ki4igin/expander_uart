#include "adc_ex.h"

#define ADC_BATTERY_MAX_VOLTAGE 17.0f
#define ADC_SENS_THRESHOLD_12B 3685

static uint32_t queue[9] = {5,6,7,8,0,1,2,3,4};

void adc_meas(int16_t *dest)
{
	LL_ADC_REG_StartConversionSWStart(ADC1);
	for(uint32_t i = 0; i < ADC_CH_CNT; i++)
	{
		while(!LL_ADC_IsActiveFlag_EOCS(ADC1));
		//LL_ADC_ClearFlag_EOCS(ADC1);
		*(dest + queue[i]) = LL_ADC_REG_ReadConversionData8(ADC1);
	}
}

uint16_t adc_get_sens_state(int16_t *src)
{
    uint16_t val = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        uint16_t state = *(src++) < ADC_SENS_THRESHOLD_12B;
        val |= (0x3*state << 2*i);
    }
    return val;
}


float adc_convert_to_voltage(uint16_t v)
{
	return v*ADC_BATTERY_MAX_VOLTAGE/4095.0f;
}
