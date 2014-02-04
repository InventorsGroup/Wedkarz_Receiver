#include "time.h"

#define T1S 30
#define T2S 80
#define T10S 500
#define T20S 50
#define T250MS 7
#define T500MS 15
#define T35MS 1

volatile unsigned int contact_counter = 0;
volatile unsigned char contact_counter2 = 0;
volatile unsigned int t10s = 0;
volatile unsigned int t500ms = 0;
volatile int fb_led = -1;
volatile unsigned char bite_blink = 0;

unsigned int stan_poprzedni=1; //Przy inicjalizacji bedzie to 'seed'
const unsigned int x=22695477, c=1;
volatile unsigned char state = -1;
volatile unsigned char d = 0;
volatile unsigned char blinker = 0;


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
	if (blinker == T250MS)
	{
		state*=-1;
		blinker = 0;
	}
	
	if (power_btn > 0)
	{
		if (!(B1_PIN & (1 << B1))) power_btn++;	
		else
		{
			power_btn = 0;
			if (func_mode == 0)
			{
				main_mode++;
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
			led_clear();
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
			device = 0;
			func_mode = 1;
			function = 2;
			for (int i = 0; i < 4; i++)
			{
				bite[i] = 0;
				led_set(11,0);
				led_set(12,0);
				led_set (i,0);
			}	
		}
		else 
		{
			//tu wjebać zapis do epromu id_tab
			led_clear();
			func_mode = 0;
			wait_for_pair = 0;
			
		}
	}
	
	
	 if (t500ms == T500MS)
	 {
		if (func_mode == 1)
		{
			fb_led *= -1;
			send(1, 0);
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

	if (bite_blink == T250MS)
	{
		if ((main_mode > 1) && (func_mode == 0))
		{
			for (int i = 0; i < 4 ; i++)
			{
				if (bite[i] >= T20S)
				{
					bite[i] = 0;
					led_set(11,0);
					led_set(12,0);
					led_set (i,0);
				}
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
					bite[i]++;
				}
			}
		}
		bite_blink = 0;
	}
	
	if (contact_counter == T500MS)
	{
		if ((main_mode == 1) && (func_mode == 0))
		{
			if (d > 3) d = 0;
			send(2, d);
			d++;
		}
		contact_counter = 0;
	}
	
	
	for (int i = 0; i < 4; i++)
	{
		if (contact[i] > 0)
		{
			contact[i]++;
		}
		if (contact[i] >= T2S)
		{
			contact[i] = 0;
		}	
	}

	led_counter++;
	bite_blink++;
	led_set(10, func_mode*fb_led);
	led_push();
	contact_counter++;
	t10s++;
	t500ms++;
	blinker++;
	func_timer++;
	
	
	
}
