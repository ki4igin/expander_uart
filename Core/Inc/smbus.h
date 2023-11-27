#ifndef __SMBUS_H__
#define __SMBUS_H__

#include "stdint.h"
#include "stm32f4xx_ll_i2c.h"

extern volatile uint32_t smbus_is_busy;

void smbus_write(uint8_t slaw, uint8_t cmd, uint16_t data);
void smbus_read(uint8_t slar, uint8_t cmd);

void smbus_write_callback(void);
void smbus_read_callback(uint16_t recv_data);

#endif /* __I2C_H__ */
