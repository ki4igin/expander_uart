#include "smbus_fifo.h"
#include "smbus.h"
#include "stm32f4xx_ll_i2c.h"
#include "fifo.h"

#define slaw(_sla) ((_sla) << 1)
#define slar(_sla) (((_sla) << 1) | 0x01)

fifo_declare(fifo_send, 4);
#define fifo_send (struct fifo *)fifo_send_buf

fifo_declare(fifo_read, 4);
#define fifo_read (struct fifo *)fifo_read_buf

union send_data {
    struct {
        uint8_t slawr;
        uint8_t cmd;
        uint16_t data;
    };

    uint32_t raw;
};

static void send_next(void)
{
    if (fifo_is_nonempty(fifo_send)) {
        union send_data req = {.raw = fifo_pop(fifo_send)};
        if (req.slawr | 0x01) {
            smbus_read(req.slawr, req.cmd);
        } else {
            smbus_write(req.slawr, req.cmd, req.data);
        }
    }
}

void smbus_fifo_write(uint8_t sla, uint8_t cmd, uint16_t data)
{
    if (smbus_is_busy()) {
        union send_data s = {.slawr = slaw(sla), .cmd = cmd, .data = data};
        fifo_push(fifo_send, s.data);
    } else {
        smbus_write(slaw(sla), cmd, data);
    }
}

void smbus_fifo_read(uint8_t sla, uint8_t cmd, struct smbus_read_data *data)
{
    if (smbus_is_busy()) {
        union send_data s = {.slawr = slar(sla), .cmd = cmd};
        fifo_push(fifo_send, s.data);
    } else {
        smbus_read(slar(sla), cmd);
    }
    fifo_push(fifo_read, (uint32_t)data);
}

void smbus_write_callback(void)
{
    send_next();
}

void smbus_read_callback(uint16_t recv_data)
{
    struct smbus_read_data *r = (struct smbus_read_data *)fifo_pop(fifo_read);
    r->is_ready = 1;
    r->val = recv_data;
    send_next();
}
