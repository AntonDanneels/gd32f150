#include "gd32f150_gpio.h"

typedef struct {
    volatile uint32_t ctl;
    volatile uint32_t omode;
    volatile uint32_t ospeed;
    volatile uint32_t pud;
    volatile uint32_t istat;
    volatile uint32_t octl;
    volatile uint32_t bop;
    volatile uint32_t lock;
    volatile uint32_t afsel0;
    volatile uint32_t afsel1;
    volatile uint32_t bc;
} gpio_peripheral;

volatile gpio_peripheral* const gpioa = (gpio_peripheral *)0x48000000;
volatile gpio_peripheral* const gpiob = (gpio_peripheral *)0x48000400;

static volatile gpio_peripheral *gpio_bank_to_peripheral(enum gpio_bank bank)
{
    switch(bank) {
        case GPIO_BANK_A:
            return gpioa;
        case GPIO_BANK_B:
            return gpiob;
        case GPIO_BANK_C:
            return 0;
        case GPIO_BANK_D:
            return 0;
        default:
            return 0;
    }
}

void gpio_setup_pin(const gpio_pin *pin, gpio_params *params)
{
    uint32_t ctl, pud, omode, ospeed, afsel;
    volatile gpio_peripheral *gpio = gpio_bank_to_peripheral(pin->bank);

    ctl = gpio->ctl;
    ctl &= ~(0x3 << (pin->gpio_pin * 2));
    ctl |= (params->mode << (pin->gpio_pin * 2));
    gpio->ctl = ctl;

    omode = gpio->omode;
    omode &= ~(0x1 << pin->gpio_pin);
    omode |= (params->output_mode << pin->gpio_pin);
    gpio->omode = omode;

    ospeed = gpio->ospeed;
    ospeed &= ~(0x3 << (pin->gpio_pin * 2));
    ospeed |= (params->speed << (pin->gpio_pin * 2));
    gpio->ospeed = ospeed;

    pud = gpio->pud;
    pud &= ~(0x3 << (pin->gpio_pin * 2));
    pud |= (params->pud_mode << (pin->gpio_pin * 2));
    gpio->pud = pud;

    if (params->mode == GPIO_ALTERNATE) {
        if (pin->gpio_pin <= 7) {
            afsel = gpio->afsel0;
            afsel &= ~(0xf << (pin->gpio_pin * 4));
            afsel |= (params->af_mode << (pin->gpio_pin * 4));
            gpio->afsel0 = afsel;
        } else {
            afsel = gpio->afsel1;
            afsel &= ~(0xf << ((pin->gpio_pin - 8) * 4));
            afsel |= (params->af_mode << ((pin->gpio_pin - 8)* 4));
            gpio->afsel1 = afsel;
        }
    }
}

void gpio_output_set_high(const gpio_pin *pin)
{
    uint32_t bop;
    volatile gpio_peripheral *gpio = gpio_bank_to_peripheral(pin->bank);

    bop = gpio->bop;
    bop |= (1 << pin->gpio_pin);
    gpio->bop = bop;
}

void gpio_output_set_low(const gpio_pin *pin)
{
    uint32_t bop;
    volatile gpio_peripheral *gpio = gpio_bank_to_peripheral(pin->bank);

    bop = gpio->bop;
    bop |= (1 << (pin->gpio_pin + 16));
    gpio->bop = bop;
}

uint8_t gpio_input_read(const gpio_pin *pin)
{
    volatile gpio_peripheral *gpio = gpio_bank_to_peripheral(pin->bank);

    return (gpio->istat >> pin->gpio_pin) & 0x1;
}
