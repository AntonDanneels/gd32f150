#include "gd32f150_spi.h"

typedef struct {
    volatile uint32_t ctl0;
    volatile uint32_t ctl1;
    volatile uint32_t stat;
    volatile uint32_t data;
    volatile uint32_t crc_poly;
    volatile uint32_t crc_rx;
    volatile uint32_t crc_tx;
    volatile uint32_t i2s_ctl;
} spi_peripheral;

volatile spi_peripheral * const spi1 = (spi_peripheral *)0x40013000;
const uint8_t SPI_CTL_CLK_MODE  = 0;
const uint8_t SPI_CTL_MSTMOD    = 2;
const uint8_t SPI_CTL_PSC_BIT   = 3;
const uint8_t SPI_CTL_ENABLE    = 6;
const uint8_t SPI_CTL_LSB_MODE  = 7;
const uint8_t SPI_CTL_SWNSS     = 8;
const uint8_t SPI_CTL_SWNSSEN   = 9;
const uint8_t SPI_CTL_RECVONLY  = 10;
const uint8_t SPI_CTL_FF16      = 11;
const uint8_t SPI_CTL_BDOEN     = 14;
const uint8_t SPI_CTL_BDEN      = 15;

void spi_init(spi_init_params *params)
{
    uint32_t ctl0, i2s_ctl;

    ctl0 = spi1->ctl0;

    ctl0 &= ~(0x3 << SPI_CTL_PSC_BIT);
    ctl0 |= (params->clk_psc << SPI_CTL_PSC_BIT);

    ctl0 &= ~(1 << SPI_CTL_FF16);
    ctl0 |= (params->frame_format << SPI_CTL_FF16);

    ctl0 &= ~(0x3 << 0);
    ctl0 |= params->mode;

    ctl0 &= ~(1 << SPI_CTL_LSB_MODE);
    ctl0 |= (params->frame_format << SPI_CTL_LSB_MODE);

    /* TODO: figure out what to do with this NSS */
    ctl0 |= (0x1 << SPI_CTL_SWNSSEN);
    ctl0 |= (0x1 << SPI_CTL_SWNSS);

    ctl0 &= ~(0x1 << SPI_CTL_MSTMOD);
    ctl0 &= ~(0x1 << SPI_CTL_RECVONLY);
    ctl0 &= ~(0x1 << SPI_CTL_BDOEN);
    ctl0 &= ~(0x1 << SPI_CTL_BDEN);
    switch (params->type) {
        case SPI_MASTER_DUPLEX:
            ctl0 |= (0x1 << SPI_CTL_MSTMOD);
            break;
        case SPI_MASTER_TRANSMIT:
            ctl0 |= (0x1 << SPI_CTL_MSTMOD);
            break;
        case SPI_MASTER_RECEIVE:
            ctl0 |= (0x1 << SPI_CTL_MSTMOD);
            break;
    }

    ctl0 |= (0x1 << SPI_CTL_ENABLE);
    spi1->ctl0 = ctl0;

    i2s_ctl = spi1->i2s_ctl;
    i2s_ctl = 0;
    spi1->i2s_ctl = i2s_ctl;
}

static uint8_t spi_get_flag(uint32_t reg, uint32_t flag)
{
    return (reg >> flag) & 0x1;
}

const uint32_t SPI_STAT_RBNE = 0x0;
const uint32_t SPI_STAT_TBE  = 0x1;

void spi_write_byte(uint8_t byte)
{
    uint32_t stat = spi1->stat;
    while(!spi_get_flag(spi1->stat, SPI_STAT_TBE)) {}
    spi1->data = byte;
    while(!spi_get_flag(spi1->stat, SPI_STAT_RBNE)) {}
    uint8_t response = spi1->data;
}
