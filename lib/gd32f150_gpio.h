#ifndef GD32F150_GPIO_H
#define GD32F150_GPIO_H

#include <stdint.h>

enum gpio_mode {
    GPIO_INPUT      = 0x00,
    GPIO_OUTPUT     = 0x01,
    GPIO_ALTERNATE  = 0x02,
    GPIO_ANALOG     = 0x03,
};

enum gpio_bank {
    GPIO_BANK_A,
    GPIO_BANK_B,
    GPIO_BANK_C,
    GPIO_BANK_D
};

enum gpio_speed {
    GPIO_SPEED_2M   = 0x00,
    GPIO_SPEED_10M  = 0x01,
    GPIO_SPEED_50M  = 0x02,
};

enum gpio_output_mode {
    GPIO_OUTPUT_PUSH_PULL  = 0x0,
    GPIO_OUTPUT_OPEN_DRAIN = 0x1,
};

enum gpio_pud_mode {
    GPIO_PULL_UP    = 0x01,
    GPIO_PULL_DOWN  = 0x02,
    GPIO_FLOAT      = 0x00,
};

enum gpio_af_mode {
    GPIO_AF_0   = 0x0,
    GPIO_AF_1   = 0x1,
    GPIO_AF_2   = 0x2,
    GPIO_AF_3   = 0x3,
    GPIO_AF_4   = 0x4,
    GPIO_AF_5   = 0x5,
    GPIO_AF_6   = 0x6,
    GPIO_AF_7   = 0x7,
};

typedef struct {
    enum gpio_bank bank;
    uint8_t gpio_pin;
} gpio_pin;

typedef struct {
    enum gpio_mode mode;
    enum gpio_output_mode output_mode;
    enum gpio_pud_mode pud_mode;
    enum gpio_speed speed;
    enum gpio_af_mode af_mode;
} gpio_params;

void gpio_setup_pin(const gpio_pin *pin, gpio_params *params);
void gpio_output_set_high(const gpio_pin *pin);
void gpio_output_set_low(const gpio_pin *pin);
uint8_t gpio_input_read(const gpio_pin *pin);

static const gpio_pin PA0  = { .bank = GPIO_BANK_A, .gpio_pin = 0 };
static const gpio_pin PA1  = { .bank = GPIO_BANK_A, .gpio_pin = 1 };
static const gpio_pin PA2  = { .bank = GPIO_BANK_A, .gpio_pin = 2 };
static const gpio_pin PA3  = { .bank = GPIO_BANK_A, .gpio_pin = 3 };
static const gpio_pin PA5  = { .bank = GPIO_BANK_A, .gpio_pin = 5 };
static const gpio_pin PA6  = { .bank = GPIO_BANK_A, .gpio_pin = 6 };
static const gpio_pin PA7  = { .bank = GPIO_BANK_A, .gpio_pin = 7 };
static const gpio_pin PA10 = { .bank = GPIO_BANK_A, .gpio_pin = 10 };
static const gpio_pin PB0  = { .bank = GPIO_BANK_B, .gpio_pin = 0 };
static const gpio_pin PB1  = { .bank = GPIO_BANK_B, .gpio_pin = 1 };

#endif
