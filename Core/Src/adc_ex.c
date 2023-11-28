#include "adc_ex.h"

#define ADC_BATTERY_MAX_VOLTAGE 18.0f
#define ADC_SENS_THRESHOLD_12B  2500
#define VOLTAGE_OFFSET          8

extern uint16_t adc_buf[ADC_CH_COUNT];

uint16_t adc_get_sens_state(uint16_t *src)
{
    uint16_t val = 0;
    for (uint32_t i = 0; i < 8; i++) {
        uint16_t state = *(src++) > ADC_SENS_THRESHOLD_12B;
        val |= (0x3 * state << 2 * i);
    }
    return val;
}

float adc_get_voltage(uint16_t *src)
{
    return *(src + VOLTAGE_OFFSET) * ADC_BATTERY_MAX_VOLTAGE / 4095.0f;
}

void adc_dma_irq_callback(void)
{
    adc_sens_data_ready_callback(ADC_CH_SENS_COUNT, &adc_buf[ADC_CH_SENS1]);
    adc_bat_data_ready_callback(adc_buf[ADC_CH_BAT]);
}

__WEAK void adc_sens_data_ready_callback(uint32_t sens_count,
                                         uint16_t data[static sens_count])
{
    (void)sens_count;
    (void)data;
}

__WEAK void adc_bat_data_ready_callback(uint16_t data)
{
    (void)data;
}
