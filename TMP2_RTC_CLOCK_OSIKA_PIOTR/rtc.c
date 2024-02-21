#include "rtc.h"
#include "MKL05Z4.h"

void rtc_init(void){
	int i;
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

	SIM->SOPT1 = SIM_SOPT1_OSC32KSEL(0);
	NVIC_DisableIRQ(RTC_IRQn);
 
  RTC->CR  = RTC_CR_SWR_MASK;
  RTC->CR  &= ~RTC_CR_SWR_MASK;  
  if (RTC->SR & RTC_SR_TIF_MASK){
        RTC->TSR = 0x00000000;   //  czyszczenie TIF
    }
		/*Wlaczenie oscylatora*/
  RTC->CR |= RTC_CR_OSCE_MASK|RTC_CR_SC16P_MASK;

  /*czekanie az sie ustabilizuje*/
  for(i=0;i<0x600000;i++);
  RTC->SR |= RTC_SR_TCE_MASK;
  
	//KOMPENSACJA CZASU
	RTC->TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);
		
	// enable rtc i wpisz 1 do rejestru
	RTC->SR &= ~(RTC_SR_TCE_MASK);
	RTC->TSR = 1;
	RTC->SR |= RTC_SR_TCE_MASK;
		
	
// przerwanie na alarmie
	RTC->IER |= RTC_IER_TAIE_MASK;
		
		
	NVIC_ClearPendingIRQ(RTC_IRQn);        
	NVIC_EnableIRQ(RTC_IRQn);
}

void rtc_set_alarm(uint32_t alarm){
		
	  if (alarm >0) {
    RTC->TAR = alarm; //wpisanie do TAR czysci flage TAF
     }
  }


uint32_t rtc_read(void) {
	return RTC->TSR;
}

void rtc_write(uint32_t t) {

	RTC->SR &= ~RTC_SR_TCE_MASK;
	if(t==0){ 
		t= 1;
		}
	RTC->TSR = t;

	RTC->SR |= RTC_SR_TCE_MASK;
}
