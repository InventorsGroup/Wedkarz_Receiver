#include "time.h"

#define T1S 30
#define T2S 80
#define T10S 700
#define TSLEEP 2000
#define T20S 50
#define T250MS 7
#define T500MS 15
#define T35MS 1
#define TVIB 10


void time_init()
{
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 255;
}

ISR(TIMER0_COMPA_vect)
{
	if (bite > 0)
	{
		bite++;
	}
	
	if (bite == T10S)
	{
		bite = 0;
	}
}

