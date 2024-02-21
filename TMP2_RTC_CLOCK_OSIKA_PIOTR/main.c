//Budzik: nastawiany aktualny czas i czas budzenia, drzemka (ustawiany interwał czasowy ), 
//alarm w postaci migających diod LED i syrenie (generator DDS). , wyłączanie alarmu polem dotykowym – zegar z wykorzystaniem RTC.
//Piotr Osika

#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw.h"
#include "alarm.h"
#include "rtc.h"
#include "clock.h"
#include "alarm.h"
#include "DAC.h"
#include "tsi.h"
#include "epoch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SNOOZE_REPEATS	10						
#define DIV_CORE	4096

volatile uint8_t S1_press=0;
volatile uint8_t S2_press=0;
volatile uint8_t S3_press=0;
volatile uint8_t S4_press=0;
volatile uint8_t alarm_on=0;
volatile uint8_t set_hours = 0;
volatile uint8_t set_time = 0;
volatile uint8_t set_alarm = 0;
volatile uint8_t set_snooze = 0;
volatile uint8_t set_enable = 0;
volatile uint8_t alarm_added = 0;
uint32_t alarm_time = 0;

int32_t Trojkat[]={0,409,818,1227,1636,2045,1636,1227,818,409,0,-409,-818,-1227,-1636,-2045,-1636,-1227,-818,-409};
uint16_t dac;
uint8_t n;
uint8_t snooze_time = 0;	
	
void SysTick_Handler(void)	// Podprogram obsługi przerwania od SysTick'a
{
	if(alarm_on){
		dac=(Trojkat[n]*100)/100+0x0800;			// Przebieg trójkątny
		DAC_Load_Trig(dac);
		n+=1;
		if(n>19)	// Czy wysłano 20 próbek?
			n=0;
   }
}
	


void RTC_IRQHandler(void)
{	
	alarm_on=1;		
	rtc_set_alarm(alarm_time);			      //zerowanie flagi przerwania alarmu																	
	NVIC_ClearPendingIRQ(RTC_IRQn);       //kaswoanie niechcianych(nieprzewidywanych) przerwan RTC
											
}

void PORTA_IRQHandler(void)	
{
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK | S3_MASK | S4_MASK);

	switch(buf)
	{
		case S1_MASK:   DELAY(10);                   
									if(!(PTA->PDIR&S1_MASK))		
									{
										if(!(PTA->PDIR&S1_MASK))	
										{
											if(!S1_press)
											{ 
												
												S1_press=1;
											}
										}
									}
									break;
		case S2_MASK:	DELAY(10);
									if(!(PTA->PDIR&S2_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S2_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S2_press)
											{
												S2_press=1;
											}
										}
									}
									break;
		case S3_MASK:	DELAY(10);
									if(!(PTA->PDIR&S3_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S3_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S3_press)
											{
												S3_press=1;
											}
										}
									}
									break;
		case S4_MASK:	DELAY(10);
									if(!(PTA->PDIR&S4_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S4_MASK)){	// Minimalizacja drgan zestyków (c.d.)
		
										 if(!S4_press)
										{
											S4_press=1;
										}
									}}
								
									break;
		default:			break;
	}	
	PORTA->ISFR |=  S1_MASK | S2_MASK | S3_MASK | S4_MASK;	// Kasowanie wszystkich bitów ISF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}

int main (void) 
{
	uint32_t rtc_time = 0;
	uint8_t w;
	uint8_t hour = 0;
  uint8_t minute = 0;
	uint8_t ahour = 0;
  uint8_t aminute = 0;
	uint8_t snooze_pressed = 0;
	
	
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	
	
	Klaw_Init();                         // inicjalizacja kalwiatury
	Klaw_S2_4_Int();                     // przerwania klawiatury
	LCD1602_Init();		                   // Inicjalizacja LCD
	LCD1602_Backlight(TRUE);
	LED_Init();        									 // inicjalizacja diody led i portu pod ktoóry podpiety jest buzzer
	TSI_Init();				                   // Inicjalizacja pola dotykowego - Slider
	rtc_init();				                   // Inicjalizacja RTC
	LCD1602_ClearAll();
	set_enable = 1;                      // odblokowanie klawiatury
	DAC_Init();	
	
	
  while(1){
		
		if(!set_time){         			//Tryb początkowy (ustawiania czasu)
			
			LCD1602_SetCursor(0,0);
			LCD1602_Print("    Set time:     ");
			if(!set_hours)
				{
				LCD1602_SetCursor(0,1);
				sprintf(display,"     __ : %02d   ", minute);
				LCD1602_Print(display);
				LCD1602_SetCursor(0,1);
				sprintf(display,"     %02d : %02d", hour, minute);
				LCD1602_Print(display);
				
				}
			else
				{
				LCD1602_SetCursor(0,1);
				sprintf(display,"     %02d : __     ", hour);
				LCD1602_Print(display);
				LCD1602_SetCursor(0,1);
				sprintf(display,"     %02d : %02d", hour, minute);
				LCD1602_Print(display); 		
				
				}
			}
			else{																				//Tryb głowny pokazujący aktualny czas hh::mm::ss
			if(!set_alarm && !alarm_on && !set_snooze)
				{
					if(rtc_time < rtc_read())
								{
								rtc_time = rtc_read();
								}
					if(alarm_added)
					{
					LCD1602_SetCursor(0,0);
					LCD1602_Print("A    It is:     ");	
					}
					else
						{
							LCD1602_SetCursor(0,0);
							LCD1602_Print("     It is:     ");
							}
						
							LCD1602_Print(display);
							LCD1602_SetCursor(0,1);
							sprintf(display,"  %02d : %02d : %02d  ", from_seconds_h(rtc_time), from_seconds_m(rtc_time), from_seconds_s(rtc_time));			
							LCD1602_Print(display);
						}
		  if(alarm_on && !set_alarm && !set_snooze)										//Tryb wywołania alarmu								
				{
				
				LCD1602_SetCursor(0,0);
				LCD1602_Print("    WAKE UP!    ");
				LCD1602_SetCursor(0,1);
				sprintf(display," Alarm: %02d : %02d ", from_seconds_h(alarm_time), from_seconds_m(alarm_time));			
				LCD1602_Print(display);
				ledOn();
				DELAY(100);
				ledOff();
				
				}				
			if(set_alarm) 																//Tryb ustawiania alarmu
				{
				LCD1602_SetCursor(0,0);
				LCD1602_Print("   Set alarm:   ");	
				if(!set_hours)
					{
					LCD1602_SetCursor(0,1);
					sprintf(display,"     __ : %02d   ", aminute);
					LCD1602_Print(display);
					LCD1602_SetCursor(0,1);
					sprintf(display,"     %02d : %02d", ahour, aminute);
					LCD1602_Print(display);
				
				}
				else if(set_hours)
					{
					LCD1602_SetCursor(0,1);
					sprintf(display,"     %02d : __     ", ahour);
					LCD1602_Print(display);
					LCD1602_SetCursor(0,1);
					sprintf(display,"     %02d : %02d", ahour, aminute);
					LCD1602_Print(display); 
				
				  }
				}
			if(set_snooze) 																//Tryb ustawiania drzemki
				{
				set_enable = 1;
				LCD1602_SetCursor(0,0);
				LCD1602_Print(" Time of snooze:   ");	
				LCD1602_SetCursor(0,1);
				sprintf(display,"       %02d s     ", snooze_time);			
				LCD1602_Print(display);
				
				}		
			
			}
			
		
		
	
			if(S1_press)                        //INKREMENTACJA WPROWADZANYCH DANYCH
				{
				
				if(set_enable)
					{	
					if (!set_hours && !set_snooze )
						{
							hour = (hour + 1) % 24;      // Zwiększenie godziny pierscieniowo
							ahour = (ahour + 1) % 24;      // Zwiększenie godziny pierscieniowo
						} 
					else if(set_hours)
						{
							minute = (minute + 1) % 60;  // Zwiększenie minut pierscieniowo
							aminute = (aminute + 1) % 60;  // Zwiększenie minut pierscieniowo
							}
					else if(set_snooze)
						{
							snooze_time++;							// Zwiększenie drzemki pierscieniowo
							}
						
				}
				
							
				  
			
				S1_press=0;
				}
			
		
			if(S2_press)                          //ZMIANA TRYBU ORAZ ZATWIERDZENIE WPROWADZANYCH DANYCH
				{
				if(set_enable)
					{
					if (!set_hours && !set_snooze)
						{
						set_hours = 1;                  // Przełączenie do ustawiania minut
						}
					else if(set_snooze)               //Akceptacja drzemki
					  {	
							SysTick_Config(SystemCoreClock/DIV_CORE);
							set_snooze =0;
							set_enable = 0;
						}
					else if(set_hours) 
						{
						if(!set_time)
							{
							set_time = 1;               //ustawienie flagi oznajmiającej że użytkownik wprowadził dane czasu
							rtc_time = to_seconds(hour,minute);    //wpisanie do rejestru RTC wprowadzonych danych
							rtc_write(rtc_time);
							ahour = 0;
							aminute =0;									
							}
						else if(set_alarm)           //ustawienie flagi oznajmiającej że użytkownik wprowadził dane alarmu
							{
							alarm_time = to_seconds(ahour,aminute);   //wpisanie do rejestru RTC wprowadzonego alarmu
							rtc_set_alarm(alarm_time);
							set_alarm = 0;
							alarm_added =1;
							set_snooze = 1;
							}
						
						set_enable = 0;
						hour = 0;
						minute =0;
						set_hours = 0;
							}
				            
					}
				S2_press=0;
			  }
		
		if(S3_press)		//USTAWIENIE ALARMU					
		{	
			if(set_time)
				{
				if(!alarm_added)
					{
						ahour = 0;
						aminute = 0;
					}
				alarm_time = 0;
				alarm_on =0;
				snooze_pressed = 0;
				rtc_set_alarm(alarm_time);
				set_enable = 1;
				set_alarm =1;
				set_hours =0;
				
				}
			S3_press=0;
		}
		
			if(S4_press)	//RESET PROGRAMU						
		{
				S4_press=0;
				alarm_time = 0;
				rtc_time = 0;
				set_alarm = 0;
				set_enable=1;
				set_hours =0;
				set_time =0;
				LCD1602_ClearAll();
				
		}
		
		w=TSI_ReadSlider();
	
		if(w!=0)               //obsługa wyłączania alarmu oraz drzemki
		{
			if(w<50 && alarm_on)
				{
				alarm_on=0;
				LCD1602_ClearAll();
				alarm_added = 0;
			  }
				
			else if(w>50 && alarm_on)
				{
					if(snooze_pressed<SNOOZE_REPEATS)
					{
						alarm_on=0;
						alarm_time = rtc_read() + snooze_time;
						rtc_set_alarm(alarm_time);
						snooze_pressed++;
					}
			   else if(snooze_pressed>SNOOZE_REPEATS)
					{
						alarm_added = 0;
						alarm_on =0;
						snooze_pressed = 0;
						snooze_time = 0;
					}
				}
		}	
		
	}
}
