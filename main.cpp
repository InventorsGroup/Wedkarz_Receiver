#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"
#include "lib/rfm12.h"
#include "lib/com.h"
#include "lib/time.h"


uint8_t *bufcontents;

volatile unsigned char f=-1;
volatile unsigned int value;
volatile unsigned char i =0;
volatile unsigned int temp;
volatile unsigned char spk_temp;



int main(void) 
 { 
	DDRC |= (1 << PC4);
	DDRC |= (1 << PC3);
	DDRD |= (1 << PD4);
	
	PORTC |=(1 << PC4);
	_delay_ms(500);
	PORTC &= ~(1 << PC4);
	
	_delay_ms(1000);
	
	rfm12_init();
	time_init();
	sei();
	
	
	while(1)
	{	
	
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			//PORTD ^= (1 << PD4);
			bufcontents = rfm12_rx_buffer();
			if (rfm12_rx_len() > 0)
			{
				parse(bufcontents);
			}
			rfm12_rx_clear();
		}
		if (bite >= 1)
		{
			PORTC |= (1 << PC3) | (1 << PC4);
			
		}
		else
		{
			PORTC &= ~(1<< PC3) & ~(1 << PC4);
		}
			
		
		rfm12_poll();
		rfm12_tick();
		
		//_delay_ms(1000);
		
	}
 }
 
 
