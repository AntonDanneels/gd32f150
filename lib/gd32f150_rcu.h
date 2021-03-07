#ifndef GD32F150_RCU_H
#define GD32F150_RCU_H

#include <stdint.h>

enum rcu_clk_domain {
    RCU_AHB  = 1,
    RCU_APB1 = 2,
    RCU_APB2 = 3,
    RCU_BD   = 4
};

enum rcu_rtc_clk {
    RCU_RTC_CLK_NONE    = 0,
    RCU_RTC_CLK_LXTAL   = 1,
    RCU_RTC_CLK_IRC40   = 2,
    RCU_RTC_CLK_HXTAL   = 3
};

static const uint32_t RCU_GPIOF_EN = 22 | (RCU_AHB << 16);
static const uint32_t RCU_GPIOD_EN = 20 | (RCU_AHB << 16);
static const uint32_t RCU_GPIOC_EN = 19 | (RCU_AHB << 16);
static const uint32_t RCU_GPIOB_EN = 18 | (RCU_AHB << 16);
static const uint32_t RCU_GPIOA_EN = 17 | (RCU_AHB << 16);

static const uint32_t RCU_USART0_EN  = 14 | (RCU_APB2 << 16);
static const uint32_t RCU_SPI0_EN    = 12 | (RCU_APB2 << 16);
static const uint32_t RCU_ADC_EN     = 9 | (RCU_APB2 << 16);

static const uint32_t RCU_PMU_EN   = 28 | (RCU_APB1 << 16);
static const uint32_t RCU_USBD_EN  = 23 | (RCU_APB1 << 16);
static const uint32_t RCU_I2C1_EN  = 22 | (RCU_APB1 << 16);
static const uint32_t RCU_I2C0_EN  = 21 | (RCU_APB1 << 16);
static const uint32_t RCU_SPI2_EN  = 15 | (RCU_APB1 << 16);
static const uint32_t RCU_SPI1_EN  = 14 | (RCU_APB1 << 16);

static const uint32_t RCU_RTC_EN   = 15 | (RCU_BD << 16);

void rcu_enable_irc40k();
void rcu_set_rtc_src(enum rcu_rtc_clk clksrc);

void rcu_enable_peripheral(const uint32_t peripheral);

#endif
