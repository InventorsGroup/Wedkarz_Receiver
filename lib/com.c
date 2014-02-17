#include "com.h"



void parse(uint8_t *data)
{
	if (data[0] == 0xFF)
	{
<<<<<<< HEAD
		switch (data[4]) 
		{
			case 0x01: // branie
				bite = 1;
				PORTD ^= (1 << PD4);
			break;
=======
			
		if (data[4] < 0x05) //only data from syg to central unit
		{	
			
			if((data[4] == 0x04) && (wait_for_pair == 1)) //answer for paring
			{
				wait_for_pair = 0;
				
				if (func_mode == 1) func_timer = 0;
				
				if ((data[1] == id_temp[0]) && (data[2] == id_temp[1]) &&(data[3] == id_temp[2]))
				{
					led_set(12,1);
					_delay_ms(100);
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
				sleep = 0;
				switch (data[4]) //wywołanie ze strony sygnalizatora
				{
					case 0x03: 
						color[k] = data[5];
						contact[k] = 1;
					break;
					
					case 0x01: // branie
						bite[k] = 1;
						bite_type[k] = data[5];
						bite_type[k] &= 0x03;
						data[5] = (data[5] >> 2);
						SPK_FREQ = data[5];
						if (main_mode > 1)
						{
							if (bite_type[k] == 1) play_speaker_alt(50);
							else play_speaker(50);
						}
					break;
					
					case 0x02:
						if (data[5] == 0x01) main_mode = -1;
						else 
						{
							main_mode = 2;
							led_set(0,0);
							led_set(1,0);
							led_set(2,0);
							led_set(3,0);
							set_speaker(0);	
						}
					break;
				}
				
			}
			known_id = 0;
>>>>>>> poprawki z bydgoszczy
		}
	}
}

<<<<<<< HEAD
=======
void send(uint8_t c, uint8_t dev, uint8_t param)
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
		case 2: //odpytanie
			command[0] = 0xFF;
			command[1] = id_tab[dev][0];
			command[2] = id_tab[dev][1];
			command[3] = id_tab[dev][2];
			command[4] = 0x0D;
			command[5] = 0x01;
			
			rfm12_tx(6, 0, command);
		break;
		
		case 3: // przesłanie nastaw
 			command[0] = 0xFF;
 			command[1] = id_tab[dev][0];
 			command[2] = id_tab[dev][1];
 			command[3] = id_tab[dev][2];
 			command[4] = 0x0A;
 			command[5] = param;
  			rfm12_tx(6, 0, command);
  		break;
		 
		case 4: //alarm!
			command[0] = 0xFF;
 			command[1] = id_tab[dev][0];
 			command[2] = id_tab[dev][1];
 			command[3] = id_tab[dev][2];
 			command[4] = 0x0B;
 			command[5] = param;
  			rfm12_tx(6, 0, command);
		break;
		
		case 5:
			command[0] = 0xFF;
 			command[1] = 0x00;
 			command[2] = 0x00;
 			command[3] = 0x00;
 			command[4] = 0x0E;
 			command[5] = 0x01;
  			rfm12_tx(6, 0, command);
		break;
  	}
>>>>>>> poprawki z bydgoszczy

