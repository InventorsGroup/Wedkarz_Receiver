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
volatile unsigned int value;
volatile unsigned char i =0;
volatile unsigned int temp;

void adc_init(void)
{
	ADCSRA  |=  (1<<ADPS2)|(1<<ADPS1);
	ADCSRA  |=  (1<<ADATE); 
}

void uart_init(uint16_t ubrr)
{
	// Ustawienie prędkości transmisji
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
 
	// Włączenie nadajnika i odbiornika
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
 
	// Ustawienie formatu ramki:
	// 8 bitów danych, 1 bit stopu, brak parzystości
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}



int main(void) 
 {  
	adc_init();
	led_init();
	button_init();
	time_init();
	rfm12_init();
	speaker_init();
	uart_init(26);

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
					
					led_set(0,fb_led*color[device]);
					led_set(1,0);
					led_set(2,0);
					led_set(3,0);
					break;
				case 1:
					led_set(0,0);
					led_set(1,fb_led*color[device]);
					led_set(2,0);
					led_set(3,0);
					break;
				case 2:
					led_set(0,0);
					led_set(1,0);
					led_set(2,fb_led*color[device]);
					led_set(3,0);
					break;
				case 3:
					led_set(0,0);
					led_set(1,0);
					led_set(2,0);
					led_set(3,fb_led*color[device]);
					break;
			}
			led_set(10, fb_led);
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
	int x = i/8;
	value = (ADCL | (ADCH << 8));
	value &= 0x01;
	temp = (temp << 1);
	temp |= (value);
	
	if (((i % 8) == 7) && (temp != 0))
	{	
		rnd[x] = temp;
	}
	i++;
	if (i > 23) i = 0;
}
