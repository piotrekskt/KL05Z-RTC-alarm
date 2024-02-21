#include "MKL05Z4.h"
#define LED_R_MASK		(1<<8)		// Maska dla diody czerwonej (R)
#define LED_R					8					// Numer bitu didody R

void LED_Init(void);
void ledOff(void);
void ledOn(void);