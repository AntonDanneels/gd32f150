#include "gd32f150_rtc.h"

#define BCD_UNIT(a) (a % 10)
#define BCD_TENS(a) (a / 10)

#define BIT(x, y) ((x >> y) & 0x1)

typedef struct {
    uint32_t time;
    uint32_t date;
    uint32_t ctl;
    uint32_t stat;
    uint32_t psc; /* addr 0x10 */
    uint32_t dummy0;
    uint32_t dummy1;
    uint32_t alarm0td; /* addr 0x1C */
    uint32_t dummy2;
    uint32_t wpk;
    uint32_t ss;
    uint32_t shiftctl;
    uint32_t tts;
    uint32_t dts;
    uint32_t ssts;
    uint32_t hrfc;
    uint32_t tamp;
    uint32_t alarm0ss;
    uint32_t bkp0;
    uint32_t bkp1;
    uint32_t bkp2;
    uint32_t bkp3;
} rtc_peripheral;

volatile rtc_peripheral * const rtc = (rtc_peripheral *)0x40002800;

static void rtc_unlock()
{
    rtc->wpk = 0xCA;
    rtc->wpk = 0x53;
}

static void rtc_lock()
{
    rtc->wpk = 0xFF;
}

void rtc_init(struct rtc_time *time)
{
    uint32_t rtc_time, rtc_date, rtc_stat, rtc_psc;
    
    rtc_unlock();

    /* Clear/wait for RSYNF */
    rtc_stat = rtc->stat;
    rtc_stat &= ~(1 << 5);
    rtc->stat = rtc_stat;

    while (!((rtc->stat >> 5) & 0x1)) {}

    /* Enter init mode & wait for it */
    rtc_stat = rtc->stat;
    rtc_stat |= (1 << 7);
    rtc_stat &= ~(1 << 8);
    rtc->stat = rtc_stat;
    while (!((rtc->stat >> 6) & 0x1)) {
      rtc_stat = rtc->stat;
    }

    /* Set prescalers, assuming IRC40k */
    rtc_psc = rtc->psc;
    rtc_psc |= (99 << 16); /* Set FACTOR_A to 99 */
    rtc_psc |= (399 << 0); /* Set FACTOR_S to 399 */
    rtc->psc = rtc_psc;

    /* Write initial calendar values */
    rtc_time = rtc->time;
    rtc_time &= ~(0xF);       /* Reset seconds (units) */
    rtc_time &= ~(0x7 << 4);  /* Reset seconds (tens) */
    rtc_time &= ~(0xF << 8);  /* Reset minutes (units) */
    rtc_time &= ~(0x7 << 12); /* Reset minutes (tens) */
    rtc_time &= ~(0xF << 16); /* Reset hours (units) */
    rtc_time &= ~(0x7 << 20); /* Reset hours (tens) */
    rtc_time |= BCD_UNIT(time->seconds);
    rtc_time |= (BCD_TENS(time->seconds) << 4);
    rtc_time |= (BCD_UNIT(time->minutes) << 8);
    rtc_time |= (BCD_TENS(time->minutes) << 12);
    rtc_time |= (BCD_UNIT(time->hours) << 16);
    rtc_time |= (BCD_TENS(time->seconds) << 20);
    rtc->time = rtc_time;

    rtc_date = rtc->date;
    rtc_date &= ~(0xF);       /* Reset days (units) */
    rtc_date &= ~(0x3 << 4);  /* Reset days (tens) */
    rtc_date &= ~(0xF << 8);  /* Reset months (units) */
    rtc_date &= ~(0x1 << 12); /* Reset months (tens) */
    rtc_date &= ~(0x7 << 13); /* Reset dow */
    rtc_date &= ~(0xF << 16); /* Reset years (units) */
    rtc_date &= ~(0xF << 20); /* Reset years (tens) */
    rtc_date |= BCD_UNIT(time->day);
    rtc_date |= (BCD_TENS(time->day) << 4);
    rtc_date |= (BCD_UNIT(time->month) << 8);
    rtc_date |= (BCD_TENS(time->month) << 12);
    rtc_date |= (BCD_TENS(time->dow) << 13);
    rtc_date |= (BCD_UNIT(time->year) << 16);
    rtc_date |= (BCD_TENS(time->year) << 20);
    rtc->date = rtc_date;

    /* Exit init mode */
    rtc_stat = rtc->stat;
    rtc_stat &= ~(1 << 7);
    rtc->stat = rtc_stat;

    rtc_lock();
}

void rtc_set_alarm(struct rtc_time *time, uint16_t mask)
{
    uint32_t rtc_ctl, rtc_alarm0td;

    rtc_unlock();

    /* Disable alarm */
    rtc_ctl = rtc->ctl;
    rtc_ctl &= ~(1 << 8);
    rtc->ctl = rtc_ctl;

    /* Check if we can write alarm */
    while (!(rtc->stat & 0x1)) {}

    /* Write alarm */
    rtc_alarm0td = rtc->alarm0td; 
    /* Clear existing masks*/
    rtc_alarm0td &= ~(0x3 << 30);
    rtc_alarm0td &= ~(0x1 << 23);
    rtc_alarm0td &= ~(0x1 << 15);
    rtc_alarm0td &= ~(0x1 << 7);
    /* Write masks*/
    rtc_alarm0td |= (BIT(mask, RTC_ALARM_SECONDS) << 7);
    rtc_alarm0td |= (BIT(mask, RTC_ALARM_MINUTES) << 15);
    rtc_alarm0td |= (BIT(mask, RTC_ALARM_HOURS) << 23);
    rtc_alarm0td |= (BIT(mask, RTC_ALARM_DAY) << 31);

    /* Clear existing values */
    rtc_alarm0td &= ~(0xF << 0); /* Seconds */
    rtc_alarm0td &= ~(0x7 << 4);
    rtc_alarm0td &= ~(0xF << 8); /* Minutes */
    rtc_alarm0td &= ~(0x7 << 12);
    rtc_alarm0td &= ~(0xF << 16); /* Hours */
    rtc_alarm0td &= ~(0x3 << 20);
    rtc_alarm0td &= ~(0xF << 24); /* Days */
    rtc_alarm0td &= ~(0x3 << 28);
    /* Write new values */
    rtc_alarm0td |= BCD_UNIT(time->seconds);
    rtc_alarm0td |= (BCD_TENS(time->seconds) << 4);
    rtc_alarm0td |= (BCD_UNIT(time->minutes) << 8);
    rtc_alarm0td |= (BCD_TENS(time->minutes) << 12);
    rtc_alarm0td |= (BCD_UNIT(time->hours) << 16);
    rtc_alarm0td |= (BCD_TENS(time->hours) << 20);
    rtc_alarm0td |= (BCD_UNIT(time->day) << 24);
    rtc_alarm0td |= (BCD_TENS(time->day) << 28);

    rtc->alarm0td = rtc_alarm0td;

    /* Enable alarm */
    rtc_ctl = rtc->ctl;
    rtc_ctl |= (1 << 8);
    rtc_ctl |= (1 << 12); /* Interrupt */
    rtc->ctl = rtc_ctl;

    rtc_lock();
}

void rtc_clear_alarm_irq()
{
  rtc_unlock(rtc);
  rtc->stat &= ~(1 << 8);
  rtc_lock(rtc);
}

uint32_t rtc_read_stat()
{
    return rtc->stat;
}

void rtc_get_time(struct rtc_time *result)
{
    uint32_t rtc_time, rtc_date;

    rtc_time = rtc->time;
    rtc_date = rtc->date;

    result->seconds = ((rtc_time >> 4) & 0x7) * 10 + (rtc_time & 0xF);
    result->minutes = ((rtc_time >> 12) & 0x7) * 10 + ((rtc_time >> 8) & 0xF);
    result->hours = ((rtc_time >> 20) & 0x3) * 10 + ((rtc_time >> 16) & 0xF);

    result->day = ((rtc_date >> 4) & 0x3) + (rtc_date & 0xF);
    result->month = ((rtc_date >> 12) & 0x1) + ((rtc_date >> 8) & 0xF);
    result->year = ((rtc_date >> 20) & 0xF) + ((rtc_date >> 16) & 0xF);

    result->dow = ((rtc_date >> 13) & 0x7);
}
