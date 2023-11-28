#ifndef __SMBUS_FIFO_H__
#define __SMBUS_FIFO_H__

#include "stdint.h"

struct smbus_read_data {
    uint16_t is_ready;
    uint16_t val;
};

void smbus_fifo_write(uint8_t sla, uint8_t cmd, uint16_t data);
void smbus_fifo_read(uint8_t sla, uint8_t cmd, volatile struct smbus_read_data *data);

#endif /* __I2C_H__ */
