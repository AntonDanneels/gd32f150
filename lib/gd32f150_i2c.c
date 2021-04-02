#include "gd32f150_i2c.h"

typedef struct {
    volatile uint32_t ctl0;
    volatile uint32_t ctl1;
    volatile uint32_t saddr0;
    volatile uint32_t saddr1;
    volatile uint32_t data;
    volatile uint32_t stat0;
    volatile uint32_t stat1;
    volatile uint32_t clk_cfg;
    volatile uint32_t rise_t;
} i2c_peripheral;

volatile i2c_peripheral *const i2c0 = (i2c_peripheral *)0x40005400U;

static const uint32_t I2C_SBSEND = 0x0;
static const uint32_t I2C_ADDSEND = 0x1;
static const uint32_t I2C_RBNE = 0x6;
static const uint32_t I2C_TBNE = 0x7;

void i2c_init(struct i2c_init_params *params)
{
    uint32_t ctl0, ctl1, ckcfg, saddr0;

    /* Config the I2c clock, since we did not configure the global clock,
     * it will still use the default clock = internal 8MHz RC osc */
    ctl1 = i2c0->ctl1;
    ctl1 |= params->clk_mhz;
    i2c0->ctl1 = ctl1;
    /* Config rise time: max rise time + 1 */
    i2c0->rise_t = 0x09;
    ckcfg = i2c0->clk_cfg;
    /* Config period for standard mode=100kHz, which means:
     * Solve for CLKC: T_I2C = CLKC * T_CLK
     * */
    ckcfg |= (params->clk_mhz * 10);
    i2c0->clk_cfg = ckcfg;

    /* Slave address => needed? */
    saddr0 = i2c0->saddr0;
    saddr0 |= (0x78 << 1);
    i2c0->saddr0 = saddr0;

    /* Enable peripheral */
    ctl0 = i2c0->ctl0; 
    ctl0 |= 0x1;
    /* And enable ACK */
    ctl0 |= (0x1 << 10);
    i2c0->ctl0 = ctl0;
}

static void i2c_send_start()
{
    uint32_t ctl0;

    ctl0 = i2c0->ctl0;
    ctl0 |= (1 << 8);
    i2c0->ctl0 = ctl0;
}

static uint32_t i2c_get_flag(uint32_t flag)
{
    uint32_t stat0 = i2c0->stat0;
    uint32_t stat1 = i2c0->stat1;
        
    return (stat0 >> flag) & 0x1;
}

void i2c_write_byte(uint8_t addr, uint8_t reg, uint8_t val)
{
    uint8_t result;
    uint32_t ctl0, addr_out, stat1;
    
    i2c_send_start();
    while (!i2c_get_flag(I2C_SBSEND)) {}

    addr_out = addr << 1 | 0x0;
    i2c0->data = addr_out;

    /* clear ADDSEND by reading I2C_STAT0 & I2C_STAT1 */
    while(!i2c_get_flag(I2C_ADDSEND)) {}
    stat1 = i2c0->stat1;
    
    while(!i2c_get_flag(I2C_TBNE)) {}
    i2c0->data = reg;
    while(!i2c_get_flag(I2C_TBNE)) {}
    i2c0->data = val;
    while(!i2c_get_flag(I2C_TBNE)) {}

    /* Set STOP bit */
    ctl0 = i2c0->ctl0;
    ctl0 |= (1 << 9);
    i2c0->ctl0 = ctl0;
}

uint8_t i2c_read_byte(uint8_t addr, uint8_t reg)
{
    uint8_t result;
    uint32_t ctl0, addr_out, stat1;
    
    i2c_send_start();
    while (!i2c_get_flag(I2C_SBSEND)) {}

    addr_out = addr << 1 | 0x0;
    i2c0->data = addr_out;

    /* clear ADDSEND by reading I2C_STAT0 & I2C_STAT1 */
    while(!i2c_get_flag(I2C_ADDSEND)) {}
    stat1 = i2c0->stat1;
    
    while(!i2c_get_flag(I2C_TBNE)) {}
    i2c0->data = reg;
    while(!i2c_get_flag(I2C_TBNE)) {}

    /* Read the response by sending a repeated ACK & sending the slave addr again */
    i2c_send_start();
    while (!i2c_get_flag(I2C_SBSEND)) {}
    /* write slave address to I2C_DATA: 7 bit + last bit one(read) */
    addr_out = addr << 1 | 0x1;
    i2c0->data = addr_out;
    /* clear ADDSEND by reading I2C_STAT0 & I2C_STAT1 */
    while(!i2c_get_flag(I2C_ADDSEND)) {}
    stat1 = i2c0->stat1;

    /* We only want to read a single byte, so disable ACKEN & set STOP bit */
    ctl0 = i2c0->ctl0;
    ctl0 &= ~(1 << 10);
    ctl0 |= (1 << 9);
    i2c0->ctl0 = ctl0;

    /* Wait for RBNE */
    while(!i2c_get_flag(I2C_RBNE)) {}
    result = i2c0->data;

    return result;
}

void i2c_write(uint8_t addr, uint8_t *byte, uint8_t len)
{

}

void i2c_read(uint8_t addr, uint8_t *result, uint8_t len)
{

}
