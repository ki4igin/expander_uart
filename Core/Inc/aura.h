#ifndef __AURA_H
#define __AURA_H

#include "stm32f4xx.h"

void aura_init(void);
void aura_process(void);
void aura_measure(void);
void tim7_update_callback(void);
#endif
