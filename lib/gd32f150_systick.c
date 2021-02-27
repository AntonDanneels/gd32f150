#include "gd32f150_systick.h"

typedef struct {
    uint32_t syst_ctl_stat;
    uint32_t syst_reload_val;
    uint32_t syst_current_val;
    uint32_t syst_calib_val;
} systick_peripheral;

volatile systick_peripheral* const systick = (systick_peripheral *)0xE000E010;
volatile uint64_t ticks = 0;

void systick_init()
{
    uint32_t CLOCKFREQ = 8000000 / 1000;
    systick->syst_reload_val = CLOCKFREQ;
    systick->syst_current_val = 0;
    systick->syst_ctl_stat |= (1 << 0);
    systick->syst_ctl_stat |= (1 << 1);
    systick->syst_ctl_stat |= (1 << 2);
}

void systick_isr()
{
    ticks++;
}

void systick_delay_ms(uint32_t ms)
{
    uint64_t delta = ticks + ms;
    while (ticks < delta) {}
}
