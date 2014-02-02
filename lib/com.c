#include "com.h"

uint8_t command[6];
unsigned volatile char known_id = 0;
volatile int k;
volatile char z = -1;

volatile unsigned char wait_for_pair = 0;

void parse(uint8_t *data)
{
	if (data[0] == 0xFF)
	{
		if (data[4] < 0x05) //only data from syg to central unit
		{	
			z*=-1;
			led_set(11,z);
			if(data[4] == 0x04) //answer for paring
			{
				wait_for_pair = 0;
				
				if (func_mode == 1) func_timer = 0;
				
				if ((data[1] == id_temp[0]) && (data[2] == id_temp[1]) &&(data[3] == id_temp[2]))
				{
					led_set(12,1);
					_delay_ms(500);
					led_set(12,0);
					color[device] = data[5];
					id_tab[device][0] = id_temp[0];
					id_tab[device][1] = id_temp[1];
					id_tab[device][2] = id_temp[2];
				}
			}	
			
			for (k = 0; k<4; k++)
			{
				if ((data[1] == id_tab[k][0]) && (data[2] == id_tab[k][1]) && (data[3] == id_tab[k][2]))
				{
					known_id = 1;
					break;
				}
			}
			
			if (known_id !=0)
			{
				switch (data[4]) //wywołanie ze strony sygnalizatora
				{
					case 0x03: 
						color[k] = data[5];
					break;
				}
			}
			known_id = 0;
		}
	}
}

void send(uint8_t c)
{
	switch(c)
	{
		case 1: //parowanie z przypisaniem ID
			command[0] = 0xFF;
			command[1] = id_temp[0];
			command[2] = id_temp[1];
			command[3] = id_temp[2];
			command[4] = 0x0C;
			command[5] = 0x01;

			rfm12_tx(6, 0, command);
			
		break;
	}
} 