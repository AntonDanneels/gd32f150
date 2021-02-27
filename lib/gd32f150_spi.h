#ifndef GD32F150_SPI_H
#define GD32F150_SPI_H

#include <stdint.h>

enum spi_data_format {
    SPI_DATA_FORMAT_8  = 0x0,
    SPI_DATA_FORMAT_16 = 0x1,
};

enum spi_frame_format {
    SPI_FRAME_FORMAT_MSB = 0x0,
    SPI_FRAME_FORMAT_LSB = 0x1,
};

enum spi_mode {
    SPI_MODE_0 = 0x00, /* CKP = 0, CPH = 0 */
    SPI_MODE_1 = 0x01, /* CKP = 0, CPH = 1 */
    SPI_MODE_2 = 0x10, /* CKP = 1, CPH = 0 */
    SPI_MODE_3 = 0x11, /* CKP = 1, CPH = 1 */
};

/* TODO: extend when more roles are needed */
enum spi_type {
    SPI_MASTER_DUPLEX,
    SPI_MASTER_TRANSMIT,
    SPI_MASTER_RECEIVE
};

typedef struct {
    uint8_t clk_psc;
    enum spi_data_format data_format;
    enum spi_frame_format frame_format;
    enum spi_mode mode;
    enum spi_type type;
} spi_init_params;

void spi_init(spi_init_params *param);
void spi_write_byte(uint8_t byte);

#endif
