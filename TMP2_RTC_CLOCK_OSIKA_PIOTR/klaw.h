#ifndef KLAW_H
#define KLAW_H

#include "MKL05Z4.h"
#define S1_MASK	(1<<7)		// Maska dla klawisza S1  inkrementujacego wprowadzanie godziny
#define S2_MASK	(1<<10)		// Maska dla klawisza S2  pierwsze klikniecie pozwalajacego na zmiane z godziny na minuty przy wprowadzaniu czasu                         //												drugie klikniecie akcepruje wprowadzony czas i przechodzi do wyswietlalnia aktualnego czasu								
#define S3_MASK	(1<<11)		// Maska dla klawisza S3  przechodzi do trybu ustawienia alarmu ( wtedy S1 i S2 dzialaja tak jak przy wprowadzaniu godziny)
#define S4_MASK	(1<<12)		// Maska dla klawisza S4  RESET URZADZENIA	
#define S1	7							// Numer bitu dla klawisza S1
#define S2	10						// Numer bitu dla klawisza S2
#define S3	11						// Numer bitu dla klawisza S3
#define S4	12						// Numer bitu dla klawisza S4

void Klaw_Init(void);
void Klaw_S2_4_Int(void);

#endif  /* KLAW_H */

