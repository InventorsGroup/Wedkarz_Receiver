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
volatile char f=-1;

void adc_init(void)
{
	ADCSRA  |=  (1<<ADPS1)|(1<<ADPS0)|(1<<ADPS2); 
	ADCSRA  |=  (1<<ADEN); 
	ADCSRA  |=  (1<<ADATE); 
	ADCSRA  |=  (1<<ADIE); 
	ADCSRA  |= (1<<ADSC);  
}

int main(void) 
 {  
	
	led_init();
	button_init();
	time_init();
	rfm12_init();
	
	//adc_init();
	sei();
	power_up();
	power_down();
	
	
	while(1)
	{	
		led_enable(1);
		if (power_flag == 0)
		{		
			power_down();
		}
		else
		{
			
			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
				led_set(11,f);
				f*=-1;
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
			
<<<<<<< HEAD
		}

		
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
			
		}
		else
		{
			led_set(4, 0);
			led_set(5, 0);
			led_set(6, 0);
			
			if (main_mode == 1)
			{
				led_set(11,0);
				led_set(12,0);
				led_set(9,1);
				led_set(7,0);
				for (int i = 0; i < 4; i++)
				{
					if (contact[i] > 0) led_set(i, state*color[i]);
					else led_set(i, 0);
				}
			}
			else led_set (9,0); 
			
			if (main_mode > 1) 
			{
				if (main_mode == 2) led_set(7,1);
				else led_set (7,0); 
				for(int i =0; i <4; i++)
				{
					if (bite[i] > 0)
					{
						led_set(i, state*color[i]);
						if (bite_type[i] == 1)
						{
							led_set(11,1);
							led_set(12,0);
						}
						else
						{
							led_set(11,0);
							led_set(12,1);
						}
						
					}
				}
			}	
		}
		
		led_set(10, func_mode*fb_led);
		
=======
		}		
>>>>>>> parent of acd8d37... Fixes
		rfm12_poll();
		rfm12_tick();	
	}
 }


ISR(ADC_vect)      
{
	int i = rnd_i / 2;
	
	rnd[i] =  (rnd[i]<<1);
	rnd[i] |= ADC & 0x01;   
	
	rnd_i++;
	if (rnd_i > 5) rnd_i = 0;
}


