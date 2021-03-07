#include "gd32f150_exti.h"

typedef struct {
    uint32_t inten;
    uint32_t even;
    uint32_t rten;
    uint32_t ften;
    uint32_t swiev;
    uint32_t pd;
} exti_peripheral;

volatile exti_peripheral * const exti = (exti_peripheral *)0x40010400;

void exti_enable_interrupt(uint32_t irq, enum exti_type type)
{
    uint32_t exti_inten, edge_val; 
    volatile uint32_t *edge_reg;

    exti_inten = exti->inten;
    exti_inten |= (1 << irq);
    exti->inten = exti_inten;

    switch(type) {
        case EXTI_RISING_EDGE:
            edge_reg = &exti->rten;
            break;
        case EXTI_FALLING_EDGE:
            edge_reg = &exti->ften;
            break;
        default:
            return;
    }

    edge_val = *edge_reg;
    edge_val |= (1 << irq);
    *edge_reg = edge_val;
}

void exti_clear_irq(uint32_t irq)
{
    exti->pd |= (1 << irq);
}

void exti_disable_interrupt(uint32_t irq)
{
    exti->inten &= ~(0x1 << irq);
    exti->ften &= ~(0x1 << irq);
    exti->rten &= ~(0x1 << irq);
}
