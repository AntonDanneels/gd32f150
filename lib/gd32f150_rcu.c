#include "gd32f150_rcu.h"

typedef struct {
    volatile uint32_t ctl0;
    volatile uint32_t cfg0;
    volatile uint32_t intr;
    volatile uint32_t apb2rst;
    volatile uint32_t apb1rst;
    volatile uint32_t ahben;
    volatile uint32_t apb2en;
    volatile uint32_t apb1en;
    volatile uint32_t bdctl;
    volatile uint32_t rstsck;
    volatile uint32_t ahbrst;
    volatile uint32_t cfg1;
    volatile uint32_t cfg2;
    volatile uint32_t ctl1;
} rcu_peripheral;

volatile rcu_peripheral * const rcu = (rcu_peripheral *)0x40021000;

void rcu_enable_irc40k();
void rcu_set_rtc_src(enum rcu_rtc_clk clksrc);

void rcu_enable_peripheral(const uint32_t peripheral)
{
    volatile uint32_t *reg, regval;

    switch(peripheral >> 16) {
        case RCU_AHB:
            reg = &rcu->ahben;
            break;
        case RCU_APB1:
            reg = &rcu->apb1en;
            break;
        case RCU_APB2:
            reg = &rcu->apb2en;
            break;
        case RCU_BD:
            reg = &rcu->bdctl;
            break;
        default:
          return;
    }
  
    regval = *reg;
    regval |= (1 << (peripheral & 0xffff));
    *reg = regval;
}

void rcu_enable_irc40k()
{
    rcu->rstsck |= 1; /* bit 0 of rstsck */

    /* Wait until IRC40k stable (bit 1) */
    while (!((rcu->rstsck >> 1) & 0x1)) {}
}

void rcu_set_rtc_src(enum rcu_rtc_clk clksrc)
{
    uint32_t rcu_bdctl;

    rcu_bdctl = rcu->bdctl;
    rcu_bdctl |= (clksrc << 8);
    rcu->bdctl = rcu_bdctl;
}
