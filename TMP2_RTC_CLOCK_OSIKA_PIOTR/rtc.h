#ifndef __RTC_H__
#define __RTC_H__

#include "MKL05Z4.h"



void rtc_init(void);

void rtc_write(uint32_t t);

uint32_t rtc_read(void);

void rtc_set_alarm(uint32_t time);

#endif /* __RTC_H__ */
