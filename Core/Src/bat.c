#include "bat.h"
#include "adc_ex.h"

static float voltage;


static float convert_adc2voltage(uint16_t adc)
{
    return 0.f;
}

float bat_get_voltage(void)
{
    return voltage;
}

void adc_bat_data_ready_callback(uint16_t data)
{
    voltage = convert_adc2voltage(data);
}
