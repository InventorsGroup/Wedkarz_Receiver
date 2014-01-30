#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"
#include "lib/led.h"
#include "lib/button.h"
#include "lib/time.h"
#include "lib/rfm12.h"

uint8_t *bufcontents;


int main(void) 
 {  
	led_init();
	button_init();
	time_init();
	sei();
	power_up();
	power_down();
	rfm12_init();
	
	while(1)
	{
		
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{

			

			bufcontents = rfm12_rx_buffer();

			// dump buffer contents to uart			
			if (rfm12_rx_len() > 0)
			{
				
				if (bufcontents[0] == 'a')
				{
					led_set(11,1);
					_delay_ms(500);
					led_set(11,0);
					_delay_ms(500);
				}
			}
			// tell the implementation that the buffer
			// can be reused for the next data.
			rfm12_rx_clear();

		}
		
		
		if (power_flag == 0)
		{		
			power_down();
		}
		else
		{
			
			if (main_mode == 1) led_set(9,1);
			else led_set (9,0); 
			
			if (main_mode == 2) led_set(7,1);
			else led_set (7,0); 
			
			if (func_mode == 1)
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
						led_set(0,1);
						led_set(1,0);
						led_set(2,0);
						led_set(3,0);
						break;
					case 1:
						led_set(0,0);
						led_set(1,2);
						led_set(2,0);
						led_set(3,0);
						break;
					case 2:
						led_set(0,0);
						led_set(1,0);
						led_set(2,3);
						led_set(3,0);
						break;
					case 3:
						led_set(0,0);
						led_set(1,0);
						led_set(2,0);
						led_set(3,4);
						break;
				}
				
			}
			else
			{
				led_set(4, 0);
				led_set(5, 0);
				led_set(6, 0);
				led_set(0, 0);
				led_set(1, 0);
				led_set(2, 0);
				led_set(3, 0);
			}
		}
		rfm12_tick();
		rfm12_poll();
	}
 }

