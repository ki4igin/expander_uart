#ifndef __BAT_H__
#define __BAT_H__

#include "stdint.h"

void bat_init(void);
void bat_set_charge_current(uint16_t mA);
void bat_set_input_current(uint16_t mA);
void bat_set_charge_voltage(uint16_t mV);

uint16_t bat_get_voltage(void);

#endif
