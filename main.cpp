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
#include "lib/speaker.h"


uint8_t *bufcontents;

volatile unsigned char f=-1;

void adc_init(void)
{
	ADCSRA  |=  (1<<ADPS1)|(1<<ADPS2);
	ADCSRA  |=  (1<<ADEN); 
	ADCSRA  |=  (1<<ADATE); 
	ADCSRA  |=  (1<<ADIE); 
	ADCSRA  |= (1<<ADSC);
}

int main(void) 
 {  
	adc_init();
	led_init();
	button_init();
	time_init();
	rfm12_init();
	
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
				bufcontents = rfm12_rx_buffer();
		
				if (rfm12_rx_len() > 0)
				{
					parse(bufcontents);
				}
				rfm12_rx_clear();
			}
			
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
				for (int i = 0; i < 4; i++)
				{
					if (contact[i] > 0) led_set(i, state*color[i]);
					else led_set(i, 0);
				}
			}
			else led_set (9,0); 
			
			if (main_mode == 2) led_set(7,1);
			else led_set (7,0); 
		}
		cli();
		rfm12_poll();
		rfm12_tick();	
		sei();
	}
 }
 
 
ISR(ADC_vect)      
{
	rnd =  (rnd<<1); 
	
	rnd |= ADC & 0x01;   
}
