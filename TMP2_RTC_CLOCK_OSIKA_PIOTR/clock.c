#include "clock.h"

uint32_t to_seconds(uint32_t h,uint32_t m){
	return 3600*h + 60*m;
}
uint8_t  from_seconds_h(uint32_t seconds) {
   return seconds / 3600;      // Obliczanie godzin  
}
uint8_t from_seconds_m(uint32_t seconds) {
    return (seconds % 3600) / 60; // Obliczanie minut
}
uint8_t from_seconds_s(uint32_t seconds){
    return seconds % 60;
}
