#include "time.h"

#define T1S 30
#define T10S 300
#define T500MS 15

volatile unsigned int t1s = 0;
volatile unsigned int t10s = 0;
volatile unsigned int t500ms = 0;
volatile int fb_led = -1;
unsigned int stan_poprzedni=1; //Przy inicjalizacji bedzie to 'seed'
const unsigned int x=22695477, c=1;


uint8_t losuj()
{
	stan_poprzedni=(x*stan_poprzedni+c);
	return stan_poprzedni%257;
}

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
				id_temp[0] = losuj();
				id_temp[1] = losuj();
				id_temp[2] = losuj();
				wait_for_pair = 1;
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
		else 
		{
			//tu wjebać zapis do epromu id_tab
			func_mode = 0;
		}
	}
	
	
	if (t1s == T1S)
	{
		
	}
	
	 if (t500ms == T500MS)
	 {
		if (func_mode == 1)
		{
			fb_led *= -1;
			send(1);
		}
		
		t500ms = 0;
	 }
	
	if (func_timer == T10S)
	{
		if (func_mode == 1)
		{
			func_mode =0;
			wait_for_pair = 0;
			//tu też wpierdolić zapis do eepromu
		}
	}

	
	led_set(10, func_mode*fb_led);
	led_push();
	t1s++;
	t10s++;
	t500ms++;
	func_timer++;
	
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
		led_set(0, 0);
		led_set(1, 0);
		led_set(2, 0);
		led_set(3, 0);
	}
	
}

