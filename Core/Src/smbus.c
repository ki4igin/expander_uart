#include "smbus.h"
#include "i2c.h"
#include "stm32f4xx_ll_i2c.h"
#include "stack.h"

#define I2C I2C1

volatile uint32_t smbus_is_busy = 0;

stack_declare(sm, 5);
#define stack ((struct stack *)sm_stack)

static void wr_func_start(uint16_t data)
{
    stack_clear(stack);
    stack_push(stack, data >> 8);
    stack_push(stack, data & 0xFF);
}

static void wr_func_end(uint8_t slaw, uint8_t cmd)
{
    stack_push(stack, cmd);
    stack_push(stack, slaw);
    smbus_is_busy = 1;
    LL_I2C_GenerateStartCondition(I2C);
}

void smbus_write(uint8_t slaw, uint8_t cmd, uint16_t data)
{
    wr_func_start(data);
    wr_func_end(slaw, cmd);
}

void smbus_read(uint8_t slar, uint8_t cmd)
{
    wr_func_start(0);
    stack_push(stack, slar);
    uint8_t slaw = slar & 0xFE;
    wr_func_end(slaw, cmd);
}

void I2C1_EV_IRQHandler(void)
{
    if (LL_I2C_IsActiveFlag_SB(I2C)) {
        LL_I2C_TransmitData8(I2C, stack_pop(stack));
    } else if (LL_I2C_IsActiveFlag_ADDR(I2C)) {
        if (LL_I2C_GetTransferDirection(I2C) == LL_I2C_DIRECTION_READ) {
            LL_I2C_AcknowledgeNextData(I2C, LL_I2C_NACK);
            LL_I2C_EnableBitPOS(I2C);
        } else {
            LL_I2C_EnableIT_BUF(I2C);
        }
        LL_I2C_ClearFlag_ADDR(I2C);
    } else if (LL_I2C_IsActiveFlag_TXE(I2C) && (LL_I2C_IsEnabledIT_BUF(I2C))) {
        uint32_t stack_count = stack_get_count(stack);
        if ((stack_count > 3)) {
            LL_I2C_DisableIT_BUF(I2C);
            LL_I2C_TransmitData8(I2C, stack_pop(stack));
        } else if (stack_count == 1) {
            LL_I2C_DisableIT_BUF(I2C);
            LL_I2C_TransmitData8(I2C, stack_pop(stack));
        } else {
            LL_I2C_TransmitData8(I2C, stack_pop(stack));
        }
    } else if (LL_I2C_IsActiveFlag_BTF(I2C)) {
        if (stack_get_count(stack) == 0) {
            LL_I2C_GenerateStopCondition(I2C);
            while (LL_I2C_IsActiveFlag_BUSY(I2C)) {
            }
            smbus_is_busy = 0;
            smbus_write_callback();
        } else if (LL_I2C_GetTransferDirection(I2C) == LL_I2C_DIRECTION_WRITE) {
            LL_I2C_GenerateStartCondition(I2C);
        } else {
            LL_I2C_GenerateStopCondition(I2C);
            uint16_t recv_data1 = LL_I2C_ReceiveData8(I2C);
            uint16_t recv_data2 = LL_I2C_ReceiveData8(I2C);
            uint16_t recv_data = recv_data1 + (recv_data2 << 8);

            while (LL_I2C_IsActiveFlag_BUSY(I2C)) {
            }
            smbus_is_busy = 0;
            smbus_read_callback(recv_data);
        }
    }
}

/**
 * @brief This function handles I2C1 error interrupt.
 */
void I2C1_ER_IRQHandler(void)
{
    if (LL_I2C_IsActiveSMBusFlag_TIMEOUT(I2C)) {
        LL_I2C_ClearSMBusFlag_TIMEOUT(I2C);
    }
    if (LL_I2C_IsActiveFlag_AF(I2C)) {
        LL_I2C_ClearFlag_AF(I2C);
    }
    LL_I2C_GenerateStopCondition(I2C);
}

__WEAK void smbus_write_callback(void)
{
}

__WEAK void smbus_read_callback(uint16_t recv_data)
{
    (void)recv_data;
}
