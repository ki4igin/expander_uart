#include "sens.h"
#include "adc_ex.h"

#define SENS_THRESHOLD_12B 2500

static uint16_t sens_state = 0;

uint16_t sens_get_state(void)
{
    return sens_state;
}

void adc_sens_data_ready_callback(uint32_t sens_count,
                                         uint16_t data[static sens_count])
{
    uint32_t state = 0;
    for (uint32_t i = 0; i < sens_count; i++) {
        uint32_t is_on = data[i] < SENS_THRESHOLD_12B;
        state |= (0x3 * is_on) << (2 * i);
    }
    sens_state = state;
}
