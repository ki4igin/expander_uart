#ifndef __SMBUS_H__
#define __SMBUS_H__

#include "stdint.h"

void smbus_write_callback(void);
void smbus_read_callback(uint16_t recv_data);

#endif /* __I2C_H__ */
