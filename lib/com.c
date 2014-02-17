#include "com.h"



void parse(uint8_t *data)
{
	if (data[0] == 0xFF)
	{
		switch (data[4]) 
		{
			case 0x01: // branie
				bite = 1;
				PORTD ^= (1 << PD4);
			break;
		}
	}
}


