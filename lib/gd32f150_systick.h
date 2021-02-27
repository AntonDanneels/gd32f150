#ifndef GD32F150_SYSTICK_H
#define GD32F150_SYSTICK_H

#include <stdint.h>

void systick_isr();
void systick_init();
void systick_delay_ms(uint32_t ms);

#endif
