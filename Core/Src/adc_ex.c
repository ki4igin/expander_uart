#include "adc_ex.h"

#define ADC_BATTERY_MAX_VOLTAGE 18.0f
#define ADC_SENS_THRESHOLD_12B 2500
#define VOLTAGE_OFFSET          8

uint16_t adc_get_sens_state(uint16_t *src)
{
    uint16_t val = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        uint16_t state = *(src++) > ADC_SENS_THRESHOLD_12B;
        val |= (0x3*state << 2*i);
    }
    return val;
}


float adc_get_voltage(uint16_t *src)
{
	return *(src+VOLTAGE_OFFSET)*ADC_BATTERY_MAX_VOLTAGE/4095.0f;
}
