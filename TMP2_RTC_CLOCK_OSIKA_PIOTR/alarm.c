#include "alarm.h"
void LED_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;      // Wlaczenie portu B
	PORTB->PCR[LED_R] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= LED_R_MASK ;	// Ustaw na 1 bit 8– rola jako wyjscia
	PTB->PDOR |= LED_R_MASK ;	// Zgas  diod
}

void ledOn(void){
	PTB->PTOR|=LED_R_MASK;
}

void ledOff(void){
	PTB->PTOR|=LED_R_MASK;
}
