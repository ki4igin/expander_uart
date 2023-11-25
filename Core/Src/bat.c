#include "bat.h"
#include "adc_ex.h"
#include "smbus.h"

static float voltage;

#define R1                     100e3f
#define R2                     10e3f
#define ADC_REF                3.3f
#define ADC_RESOLUTION         12

#define RES_DIV                (R2 / (R1 + R2))

#define CHARGE_CURRENT_STEP_mA 128
#define INPUT_CURRENT_STEP_mA  256
#define CHARGE_VOLTAGE_STEP_mV 16

#define CHARGE_CURRENT_Pos     7
#define INPUT_CURRENT_Pos      7
#define CHARGE_VOLTAGE_Pos     4

#define MANUFACTURER_ID        0x004D
#define DEVICE_ID              0x0008

enum cmd {
    CMD_CHARGE_CURRENT = 0x14,
    CMD_CHARGE_VOLTAGE = 0x15,
    CMD_INPUT_CURRENT = 0x3F,
    CMD_MANUFACTURER_ID = 0xFE,
    CMD_DEVICE_ID = 0xFF,
};

static uint16_t get_bits_val_charge_current(uint16_t cur_mA)
{
    return (cur_mA / CHARGE_CURRENT_STEP_mA) << CHARGE_CURRENT_Pos;
}

static uint16_t get_bits_val_input_current(uint16_t cur_mA)
{
    return (cur_mA / INPUT_CURRENT_STEP_mA) << INPUT_CURRENT_Pos;
}

static uint16_t get_bits_val_charge_voltage(uint16_t vol_mV)
{
    return (vol_mV / CHARGE_VOLTAGE_STEP_mV) << CHARGE_VOLTAGE_Pos;
}

static float convert_adc2voltage(uint16_t adc)
{
    return adc * ADC_REF / (1 << ADC_RESOLUTION) / RES_DIV;
}

void bat_init(void)
{
}

void bat_set_charge_current()
{
}

void bat_set_input_current()
{
}

void bat_set_charge_voltage()
{
}

float bat_get_voltage(void)
{
    return voltage;
}

void adc_bat_data_ready_callback(uint16_t data)
{
    voltage = convert_adc2voltage(data);
}

void smbus_write_callback(void)
{
}

void smbus_read_callback(uint16_t recv_data)
{
    (void)recv_data;
}
