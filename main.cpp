#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"
#include "lib/led.h"
#include "lib/button.h"
#include "lib/time.h"
#include "lib/rfm12.h"
#include "lib/com.h"


uint8_t *bufcontents;


int main(void) 
 {  
	led_init();
	button_init();
	time_init();
	rfm12_init();
	sei();
	power_up();
	power_down();
	
	
	while(1)
	{	
		if (power_flag == 0)
		{		
			power_down();
		}
		else
		{
			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
				bufcontents = rfm12_rx_buffer();

				// dump buffer contents to uart			
				if (rfm12_rx_len() > 0)
				{
					parse(bufcontents);
				}
				// tell the implementation that the buffer
				// can be reused for the next data.
				rfm12_rx_clear();
			}
			
		}		
		rfm12_poll();
		rfm12_tick();	
	}
 }

