#ifndef  GD32F150_RTC_H
#define GD32F150_RTC_H

#include "stdint.h"

enum rtc_dow {
    RTC_DOW_MONDAY    = 1,
    RTC_DOW_TUESDAY   = 2,
    RTC_DOW_WEDNESDAY = 3,
    RTC_DOW_THURSDAY  = 4,
    RTC_DOW_FRIDAY    = 5,
    RTC_DOW_SATURDAY  = 6,
    RTC_DOW_SUNDAY    = 7,
};

struct rtc_time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;

    uint8_t year;
    uint8_t month;
    uint8_t day;

    enum rtc_dow dow;
};

enum rtc_alarm {
    RTC_ALARM_SECONDS = 1,
    RTC_ALARM_MINUTES = 2,
    RTC_ALARM_HOURS   = 3,
    RTC_ALARM_DAY     = 4,
    RTC_ALARM_MONTH   = 5,
    RTC_ALARM_YEAR    = 6,
};

void rtc_init(struct rtc_time *time);
void rtc_set_alarm(struct rtc_time *time, uint16_t mask);
void rtc_clear_alarm_irq();
uint32_t rtc_read_stat();
void rtc_get_time(struct rtc_time *result);

#endif
