#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "MKL05Z4.h"

uint32_t to_seconds(uint32_t h,uint32_t m);
uint8_t  from_seconds_h(uint32_t seconds); 
uint8_t  from_seconds_m(uint32_t seconds);
uint8_t  from_seconds_s(uint32_t seconds);

#endif /* __CLOCK_H__ */
