#ifndef GD32F150_I2C_H
#define GD32F150_I2C_H

#include <stdint.h>

struct i2c_init_params {
    uint8_t clk_mhz;
    uint8_t master;
};

void i2c_init(struct i2c_init_params *params);

void i2c_write_byte(uint8_t addr, uint8_t reg, uint8_t val);
uint8_t i2c_read_byte(uint8_t addr, uint8_t reg);

void i2c_write(uint8_t addr, uint8_t *byte, uint8_t len);
void i2c_read(uint8_t addr, uint8_t *result, uint8_t len);

#endif
