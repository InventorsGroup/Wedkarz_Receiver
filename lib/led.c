#include "led.h"

/*
	LED Connection array
	
	Led No. / Color    R   G   B
			1
			2
			3
		   ...
*/

unsigned volatile char COLOR = 1;
unsigned volatile char BRIGHTNESS = 6;
	
volatile unsigned char led_pwr = 100;	
volatile unsigned char led_turn_off = 0;

volatile static unsigned char led_array[3][4] = {{12, 15, 18, 21},
												 {13, 16, 19, 22},
												 {14, 17, 20, 23}};


volatile static unsigned char leds[32];


void led_init()
{
	//disable leds
	led_enable(0);
	//led_power(100);
	
	//clear the array
	for (int i=0; i<36; i++) leds[i] = 0;
	
	//set DDRs
	SDI_DDR |= (1 << SDI);
	OE_DDR |= (1 << OE);
	CLK_DDR |= (1 << CLK);
	LA_DDR |= (1 << LA);
	LED_ON_DDR |= (1 << LED_ON);
	
	led_enable(1);
	
	//PWM
	TCCR1A |= (1 << COM1A1) | (1 << WGM10) | (1 << WGM12);
	TCCR1B |= (1 << CS11) | (1 << CS10);
}


void led_enable(unsigned char s)
{
	if (s > 0)
	{
		LED_ON_PORT |= (1 << LED_ON);
		_delay_ms(50);
		led_power(led_pwr);
	}
	else
	{
		LED_ON_PORT &= ~(1 << LED_ON);
		OCR1A = 255;
	}
}

void led_brightness_to_power()
{
	led_power(BRIGHTNESS * 17);
}

void led_power(unsigned char p)
{
	if (p < 0) p = 0;
	else if (p > 100) p = 100;
	
	led_pwr = p;
	OCR1A = 255-led_pwr;
}


void led_set(unsigned char n, char color)
{
	if (color < 0) color = 0;
	if (n < 4)
	{
		switch (color){
			case 0: // Off 
						leds[31-led_array[0][n]] = 0;
						leds[31-led_array[1][n]] = 0;
						leds[31-led_array[2][n]] = 0;
					break;
			case 1: // Red
						leds[31-led_array[0][n]] = 1;
						leds[31-led_array[1][n]] = 0;
						leds[31-led_array[2][n]] = 0;
					break;
			case 2: // Green
						leds[31-led_array[0][n]] = 0;
						leds[31-led_array[1][n]] = 1;
						leds[31-led_array[2][n]] = 0;
					break;
			case 3: // Blue
						leds[31-led_array[0][n]] = 0;
						leds[31-led_array[1][n]] = 0;
						leds[31-led_array[2][n]] = 1;
					break;
			case 4: // Yellow
						leds[31-led_array[0][n]] = 1;
						leds[31-led_array[1][n]] = 1;
						leds[31-led_array[2][n]] = 0;
					break;
			case 5: // Purple
						leds[31-led_array[0][n]] = 1;
						leds[31-led_array[1][n]] = 0;
						leds[31-led_array[2][n]] = 1;
					break;
			case 6: // Sky
						leds[31-led_array[0][n]] = 0;
						leds[31-led_array[1][n]] = 1;
						leds[31-led_array[2][n]] = 1;
					break;
			case 7: // White
						leds[31-led_array[0][n]] = 1;
						leds[31-led_array[1][n]] = 1;
						leds[31-led_array[2][n]] = 1;
					break;
			}
	}
		else if ((n == 4)) // exception	
		{
			if (color > 0) leds[21] = 1;
			else leds[21] = 0;
		}
		else if ((n == 5)) // exception	
		{
			if (color > 0) leds[22] = 1;
			else leds[22] = 0;
		}
		else if ((n == 6)) // exception	
		{
			if (color > 0) leds[23] = 1;
			else leds[23] = 0;
		}
		else if ((n == 7)) // exception	
		{
			if (color > 0) leds[30] = 1;
			else leds[30] = 0;
		}
		else if ((n == 8)) // exception	
		{
			if (color > 0) leds[29] = 1;
			else leds[29] = 0;
		}
		else if ((n == 9)) // exception	
		{
			if (color > 0) leds[28] = 1;
			else leds[28] = 0;
		}
		else if ((n == 10)) // exception	
		{
			if (color > 0)
			{
				leds[26] = 1;
				leds[27] = 1;
			}
			else 
			{
				leds[26] = 0;
				leds[27] = 0;
			}
		}
		else if ((n == 11)) // exception	
		{
			if (color > 0) leds[6] = 1;
			else leds[6] = 0;
		}
		else if ((n == 12)) // exception	
		{
			if (color > 0) leds[7] = 1;
			else leds[7] = 0;
		}
		
}

void led_push()
{
	for (int i = 0; i<36; i++){
		
		if (leds[i] == 1) SDI_PORT |= (1 << SDI);
		else SDI_PORT &= ~(1<<SDI);

		CLK_PORT |= (1 << CLK);
		CLK_PORT &= ~(1 << CLK);

	}
	LA_PORT |= (1 << LA);
	LA_PORT &= ~(1 << LA); // Latch output
}

void led_clear()
{
	for (int i=0; i<36; i++) leds[i] = 0;
	led_push();
}

void led_clear_no_push()
{
	for (int i=0; i<36; i++) leds[i] = 0;
}
