#include "time.h"

#define T1S 30
#define T2S 80
#define T10S 500
#define TSLEEP 2000
#define T20S 50
#define T250MS 7
#define T500MS 15
#define T35MS 1
#define TVIB 10

volatile unsigned int contact_counter = 0;
volatile unsigned char contact_counter2 = 0;
volatile unsigned int t10s = 0;
volatile unsigned int t500ms = 0;
volatile int fb_led = -1;
volatile unsigned char bite_blink = 0;

volatile char state = -1;
volatile unsigned char d = 0;
volatile unsigned char blinker = 0;
unsigned volatile int sender = 0; //USART
unsigned volatile char send_theft = 0;
unsigned volatile char cnt = 0;
unsigned volatile char cnt2 = 0;
unsigned volatile int spk_tmp = 0;
unsigned volatile char cnt3=0;

void time_init()
{
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 255;
}

ISR(TIMER0_COMPA_vect)
{

	if (vib > 0)
	{
		vib++;
	}

	if (vib == TVIB)
	{
		set_vib(0);
	}

	if(spk_cnt > 0)
	{
		spk_tmp++;
		if(spk_tmp > spk_cnt)
		{
			set_speaker(0);
			spk_tmp = 0;
			spk_cnt = 0;
		}
	}

	if (blinker == T250MS)
	{
		state*=-1;
		blinker = 0;
	}

	if (send_theft > 0)
	{
		cnt++;
		if (cnt == 20)
		{
			send(4, cnt2, send_theft);
			cnt = 0;
			cnt2++;
		}
		if(cnt2 > 3)
		{
			cnt2 = 0;
			send_theft = 0;
		}
	}

	if (theft_btn >0)
	{
		if (!((B2_PIN) & (1 << B2))) theft_btn ++;
		else
		{
			send_theft = 2;
			theft_btn = 0;
			main_mode = 2;
			led_set(0,0);
			led_set(1,0);
			led_set(2,0);
			led_set(3,0);
			set_speaker(0);
		}
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
				led_set(0,0);
				led_set(1,0);
				led_set(2,0);
				led_set(3,0);
			}
		}
	}

	if (func_btn > 0)
	{
		ADCSRA |= (1 << ADIE);
		ADCSRA |= (1 << ADEN);
		ADCSRA |= (1 << ADSC);
		if (!(B3_PIN & (1 << B3))) func_btn++;
		else
		{
			func_btn = 0;
			if (func_mode == 1)
			{
				device++;
				func_timer = 0;
				if (device > 3 ) device = 0;
				id_temp[0] = rnd[0];
				id_temp[1] = rnd[1];
				id_temp[2] = rnd[2];
				wait_for_pair = 1;
			}
			else
			{
				ADCSRA &= ~(1 << ADIE);
				ADCSRA &= ~(1 << ADEN),
				DIDR0 = 0;
			}
		}
	}

	if (theft_btn == T1S)
	{
		theft_btn = 0;
		send_theft = 1;
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
			id_temp[0] = rnd[0];
			id_temp[1] = rnd[1];
			id_temp[2] = rnd[2];
			wait_for_pair = 1;
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
			eeprom_write_byte((uint8_t*)8, (uint8_t)id_tab[0][0]);
			eeprom_write_byte((uint8_t*)16, (uint8_t)id_tab[0][1]);
			eeprom_write_byte((uint8_t*)24, (uint8_t)id_tab[0][2]);

			eeprom_write_byte((uint8_t*)32, (uint8_t)id_tab[1][0]);
			eeprom_write_byte((uint8_t*)40, (uint8_t)id_tab[1][1]);
			eeprom_write_byte((uint8_t*)48, (uint8_t)id_tab[1][2]);

			eeprom_write_byte((uint8_t*)56, (uint8_t)id_tab[2][0]);
			eeprom_write_byte((uint8_t*)64, (uint8_t)id_tab[2][1]);
			eeprom_write_byte((uint8_t*)72, (uint8_t)id_tab[2][2]);

			eeprom_write_byte((uint8_t*)80, (uint8_t)id_tab[3][0]);
			eeprom_write_byte((uint8_t*)88, (uint8_t)id_tab[3][1]);
			eeprom_write_byte((uint8_t*)96, (uint8_t)id_tab[3][2]);

			sleep = 0;
			func_mode = 0;
			wait_for_pair = 0;
			led_set(0,0);
			led_set(1,0);
			led_set(2,0);
			led_set(3,0);
			led_set(10, 0);
			ADCSRA &= ~(1 << ADIE);
			ADCSRA &= ~(1 << ADEN);
		}
	}


	 if (t500ms == T500MS)
	 {
		if (func_mode == 1)
		{
			fb_led *= -1;
			send(1, 0, 0);
		}

		t500ms = 0;
	 }

	if (func_timer == T10S)
	{
		if (func_mode == 1)
		{
			sleep = 0;
			func_mode =0;
			wait_for_pair = 0;
			led_set(0,0);
			led_set(1,0);
			led_set(2,0);
			led_set(3,0);
			led_set(10, 0);

			ADCSRA &= ~(1 << ADIE);
			ADCSRA &= ~(1 << ADEN);

			eeprom_write_byte((uint8_t*)8, (uint8_t)id_tab[0][0]);
			eeprom_write_byte((uint8_t*)16, (uint8_t)id_tab[0][1]);
			eeprom_write_byte((uint8_t*)24, (uint8_t)id_tab[0][2]);

			eeprom_write_byte((uint8_t*)32, (uint8_t)id_tab[1][0]);
			eeprom_write_byte((uint8_t*)40, (uint8_t)id_tab[1][1]);
			eeprom_write_byte((uint8_t*)48, (uint8_t)id_tab[1][2]);

			eeprom_write_byte((uint8_t*)56, (uint8_t)id_tab[2][0]);
			eeprom_write_byte((uint8_t*)64, (uint8_t)id_tab[2][1]);
			eeprom_write_byte((uint8_t*)72, (uint8_t)id_tab[2][2]);

			eeprom_write_byte((uint8_t*)80, (uint8_t)id_tab[3][0]);
			eeprom_write_byte((uint8_t*)88, (uint8_t)id_tab[3][1]);
			eeprom_write_byte((uint8_t*)96, (uint8_t)id_tab[3][2]);

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
					led_set(i, color[i]);
					if (bite_type[i] == 1)
					{
						led_set(11,state*1);
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
			send(2, d, 0);
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

	if (sleep > TSLEEP)
	{
		sleep_mode = 1;
	}

	if (func_mode == 0) sleep++;

	led_counter++;
	bite_blink++;

	led_push();
	contact_counter++;
	t10s++;
	t500ms++;
	blinker++;
	func_timer++;

	sender++; //USART
}

