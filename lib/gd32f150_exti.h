#ifndef GD32F150_EXTI_H
#define GD32F150_EXTI_H

#include <stdint.h>

enum exti_type {
    EXTI_RISING_EDGE,
    EXTI_FALLING_EDGE
};

static const uint32_t EXIT_IRQ_RTC = 17;

void exti_enable_interrupt(uint32_t irq, enum exti_type type);
void exti_clear_irq(uint32_t irq);
void exti_disable_interrupt(uint32_t irq);

#endif
