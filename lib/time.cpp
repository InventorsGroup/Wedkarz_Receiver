#include "time.h"

#define T1S 30
#define T10S 300
#define T500MS 15

volatile unsigned int t1s = 0;
volatile unsigned int t10s = 0;
volatile unsigned int t500ms = 0;
volatile int fb_led = -1;


void time_init()
{
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 255;
}

ISR(TIMER0_COMPA_vect)
{
	
	if (power_btn > 0)
	{
		if (!(B1_PIN & (1 << B1))) power_btn++;	
		else
		{
			power_btn = 0;
			if (func_mode == 0)
			{
				main_mode ++;
				if (main_mode > 3 )  main_mode = 1;
			}
		}
	}

	if (func_btn > 0)
	{
		if (!(B3_PIN & (1 << B3))) func_btn++;	
		else
		{
			func_btn = 0;
			if (func_mode == 1)
			{
				device++;
				func_timer = 0;
				if (device > 3 ) device = 0;
			}
		}
	}
	
	if (power_btn == T1S)
	{
		power_btn = 0;
		power_flag = 0;
	}
	else power_flag = 1;
	
	if (func_btn == T1S)
	{
		func_btn = 0;
		if (func_mode == 0)
		{	
			func_timer = 0;
			func_mode = 1;
			function = 2;
		}
		else func_mode = 0;
	}
	
	
	if (t1s == T1S)
	{
		
	}
	
	if (t500ms == T500MS)
	{
		if (func_mode == 1)
		{
			fb_led *= -1;
		}
		t500ms = 0;
	}
	
	if (func_timer == T10S)
	{
		if (func_mode == 1) func_mode =0;
	}
	
	led_set(10, func_mode*fb_led);
	led_push();
	t1s++;
	t10s++;
	t500ms++;
	func_timer++;
}

