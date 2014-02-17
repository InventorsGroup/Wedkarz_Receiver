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
<<<<<<< HEAD
	
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			//PORTD ^= (1 << PD4);
			bufcontents = rfm12_rx_buffer();
			if (rfm12_rx_len() > 0)
			{
				parse(bufcontents);
			}
			rfm12_rx_clear();
=======
		led_enable(1);
		
		if (power_flag == 0)
		{		
			rfm12_power_down();
					
			led_clear();
			TCCR0B = 0;//button timer 0
			PCICR = 0;

			set_speaker(0);
			
			TCCR1A = 0;
			TCCR1B = 0;
			OE_PORT &= ~(1 << OE);
			SDI_PORT &= ~(1 << SDI);
			
			ADCSRA = 0;
			DIDR0 = 0; 
			
			power_down();
>>>>>>> poprawki z bydgoszczy
		}
		if (bite >= 1)
		{
<<<<<<< HEAD
			PORTC |= (1 << PC3) | (1 << PC4);
			
		}
		else
		{
			PORTC &= ~(1<< PC3) & ~(1 << PC4);
=======
			sleep_mode = 0;
			sleep = 0;
			//go to sleep
		}
		else
		{
			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
				bufcontents = rfm12_rx_buffer();
				if (rfm12_rx_len() > 0)
				{
					parse(bufcontents);
				}
				rfm12_rx_clear();
			}
			if (main_mode == -1)
			{
				if (state == -1)
				{
					led_set(0, 1);
					led_set(1, 1);
					led_set(2, 1);
					led_set(3, 1);
					set_custom_speaker(90, 130);
					set_speaker(1);

				}
				if (state == 1)
				{
					led_set(0, 2);
					led_set(1, 2);
					led_set(2, 2);
					led_set(3, 2);
					set_custom_speaker(90, 130);
					set_speaker(0);

				}
				
			}
			else if (func_mode == 1)
			{
				switch (function)
				{
					case 0: 
						led_set(4, 1);
						led_set(5, 0);
						led_set(6, 0);
						break;
					case 1:
						led_set(4, 0);
						led_set(5, 1);
						led_set(6, 0);
						break;
					case 2:
						led_set(4, 0);
						led_set(5, 0);
						led_set(6, 1);
						break;
				}

				switch (device)
				{
					case 0:
						
						led_set(0,color[device]);
						led_set(1,0);
						led_set(2,0);
						led_set(3,0);
						break;
					case 1:
						led_set(0,0);
						led_set(1,color[device]);
						led_set(2,0);
						led_set(3,0);
						break;
					case 2:
						led_set(0,0);
						led_set(1,0);
						led_set(2,color[device]);
						led_set(3,0);
						break;
					case 3:
						led_set(0,0);
						led_set(1,0);
						led_set(2,0);
						led_set(3,color[device]);
						break;
				}
				led_set(10, fb_led);
			}
			else
			{
				led_set(4, 0);
				led_set(5, 0);
				led_set(6, 0);
				
				if (play_vol == 1)
				{
					SPK_FREQ = 1;
					play_speaker(100);
					play_vol = 0;
				}
				
				
				
				if (main_mode == 1)
				{
					led_set(11,0);
					led_set(12,0);
					led_set(9,1);
					for (int i = 0; i < 4; i++)
					{
						if (contact[i] > 0) led_set(i, state*color[i]);
						else led_set(i, 0);
					}
				}
				else led_set (9,0); 
				
				if (main_mode == 2) led_set(7,1);
				else led_set (7,0);

				// if (main_mode > 1)
				// {
					// for(int i = 0; i < 4; i++)
					// {
						// if (bite[i] > 0)
						// {
							// if ((state == 1) && (spk_temp == 0))
							// {
								// if (bite_type[i] == 2) play_speaker(50);
								// else if (bite_type[i] == 1) play_speaker_alt(50);
								
								// spk_temp = 1;
							// }
							// if (state == -1)
							// {
								// spk_temp = 0;
							// }
						// }
					// }
				// }
			}
			cli();
			rfm12_poll();
			rfm12_tick();	
			sei();
>>>>>>> poprawki z bydgoszczy
		}
			
		
		rfm12_poll();
		rfm12_tick();
		
		//_delay_ms(1000);
		
	}
 }
 
 
