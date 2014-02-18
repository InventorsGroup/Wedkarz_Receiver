#include "button.h"

volatile unsigned char power_btn = 0;
volatile char func_btn = 0;
volatile int power_flag;
volatile char main_mode;
volatile char function = 2;
volatile unsigned char pwr;
volatile unsigned char theft_btn;
volatile unsigned char play_vol = 0;

void button_init()
{
	B1_DDR &= ~(1 << B1);
	B1_PORT |= (1 << B1);

	B2_DDR &= ~(1 << B2);
	B2_PORT |= (1 << B2);

	B3_DDR &= ~(1 << B3);
	B3_PORT |= (1 << B3);

	B4_DDR &= ~(1 << B4);
	B4_PORT |= (1 << B4);

	B5_DDR &= ~(1 << B5);
	B5_PORT |= (1 << B5);

	B6_DDR &= ~(1 << B6);
	B6_PORT |= (1 << B6);

	B7_DDR &= ~(1 << B7);
	B7_PORT |= (1 << B7);

	PCICR |= (1 << PCIE2) | (1 << PCIE0); // Enable Pin change interrupts
	//Select triggers for PCINTs
	PCMSK0 |= (1 << PCINT6);
	PCMSK2 |= (1 << PCINT22) | (1 << PCINT21) | (1 << PCINT20) | (1 << PCINT19) | (1 << PCINT17) | (1 << PCINT16);
	power_flag = 1;

}

ISR(INT1_vect) //TOP (Power Button) INT
{
	//power_up();
	//led_enable(0);
}

ISR(PCINT0_vect)
{
	sleep = 0;
	char mask = (1 << B3);
	_delay_ms(10);
	if ((PINB & mask) != mask)
	{
		_delay_ms(10);
		func_btn = 1;
	}
}

ISR(PCINT2_vect)
{
	char mask = 0;
	sleep = 0;
	mask |= (1 << B1) | (1 << B2) | (1 << B4) | (1 << B5) | (1 << B6) | (1 << B7);
	_delay_ms(10);
	if ((PIND & mask) != mask)
	{
		if ((B1_PIN & (1 << B1)) == 0)
		{
			if (func_mode == 0)
			{
				power_btn = 1;
			}
		}
		if ((B4_PIN & (1 << B4)) == 0)
		{
			while (!(B4_PIN & (1 << B4)));
			func_timer = 0;
 			if (func_mode == 1)
 			{
 				switch (function)
 				{
 					case 0:
 						send(3, device, 0x31);
 						break;
 					case 1:
 						send(3, device, 0x21);
 						break;
 					case 2:
 						send(3, device, 0x11);
 						break;
 				}
 			}
			else
			{
				if (VOL > -1) VOL--;
				else VOL = -1;
				play_vol = 1;
			}
		}
		else if ((B5_PIN & (1 << B5)) == 0)
		{
			while (!(B5_PIN & (1 << B5)));
			func_timer = 0;
 			if (func_mode == 1)
 			{
 				switch (function)
 				{
 					case 0:
 						send(3, device, 0x32);
 						break;
 					case 1:
 						send(3, device, 0x22);
 						break;
 					case 2:
 						send(3, device, 0x12);
 						break;
 				}
 			}
			else
			{
				if (VOL < 4) VOL++;
				else VOL = 4;
				play_vol = 1;
			}
		}

		else if ((B6_PIN & (1 << B6)) == 0)
		{
			if (func_mode == 1)
			{
				func_timer = 0;
				function--;
				if (function < 0) function = 0;
			}
		}
		else if ((B7_PIN & (1 << B7)) == 0)
		{
			if (func_mode == 1)
			{
				func_timer = 0;
				function++;
				if (function > 2) function = 2;
			}
		}
		else if ((B2_PIN & (1 << B2)) == 0)
		{
			_delay_ms(10);
			if ((func_mode == 0) && ((main_mode > 1) || (main_mode == -1))) theft_btn = 1;
		}
		led_push();
	}
}

void power_down() //dodać zapis do eepromu
{
	eeprom_write_byte((uint8_t*)0, (uint8_t)VOL); // Save volume to eeprom

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

	cli();
	PCICR &= ~(1 << PCIE2) & ~(1 << PCIE0); // disable PCINT
	EIMSK |= (1 << INT1); // Enebale INT1 external interrupt on low state
	power_flag = 0;
	led_enable(0);
	while (!(B1_PIN & (1 << B1)));
	SMCR |= (1 << SM1) |(1 << SE);
	sleep_enable();
	sei();
	sleep_cpu();

	power_up();
	_delay_ms(50);
	if (((B1_PIN & (1 << B1)) ) !=0 )
	{
		power_flag = 0;
	}
	else
	{
		led_enable(1);
		_delay_ms(5);
		power_flag = 1;
		SPK_FREQ = 4;
		cli();
		play_speaker(50);
		sei();
	}

}

void power_up()
{
		sleep_disable();
		led_set(9,1);
		power_flag = 1;

		ADCSRA  |=  (1<<ADPS2)|(1<<ADPS1);
		ADCSRA  |=  (1<<ADATE);

		TCCR0A |= (1 << WGM01);
		TCCR0B |= (1 << CS00) | (1 << CS02);
		TIMSK0 |= (1 << OCIE0A);
		OCR0A = 255;

		led_init();
		button_init();
		vib_init();
		speaker_init();

		EIMSK &= ~(1 << INT1); //disable INT1
		PCICR |= (1 << PCIE2) | (1 << PCIE0); // enable PCINT
		rfm12_power_up();
		VOL = eeprom_read_byte((uint8_t*)0);

		id_tab[0][0] = eeprom_read_byte((uint8_t*)8);
		id_tab[0][1] = eeprom_read_byte((uint8_t*)16);
		id_tab[0][2] = eeprom_read_byte((uint8_t*)24);

		id_tab[1][0] = eeprom_read_byte((uint8_t*)32);
		id_tab[1][1] = eeprom_read_byte((uint8_t*)40);
		id_tab[1][2] = eeprom_read_byte((uint8_t*)48);

		id_tab[2][0] = eeprom_read_byte((uint8_t*)56);
		id_tab[2][1] = eeprom_read_byte((uint8_t*)64);
		id_tab[2][2] = eeprom_read_byte((uint8_t*)72);

		id_tab[3][0] = eeprom_read_byte((uint8_t*)80);
		id_tab[3][1] = eeprom_read_byte((uint8_t*)88);
		id_tab[3][2] = eeprom_read_byte((uint8_t*)96);

		sei();
		main_mode = 0;
		func_mode = 0;
		function = 2;
}
