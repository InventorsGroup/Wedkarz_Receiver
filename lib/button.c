#include "button.h"

volatile unsigned char power_btn = 0;
volatile char func_btn = 0;
volatile int power_flag;
volatile char main_mode;
volatile char func_mode = 0;
volatile char function = 2;
volatile unsigned int func_timer;
volatile unsigned char device = 0; 
volatile unsigned char pwr;

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
	power_up();
	//led_enable(0);
	_delay_ms(100);
	if (((B1_PIN & (1 << B1)) ) !=0 ) power_flag = 0;
	else 
	{
		led_enable(1);
		_delay_ms(5);
		power_flag = 1;	
	}

}

ISR(PCINT0_vect)
{
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
	
	mask |= (1 << B1) | (1 << B2) | (1 << B4) | (1 << B5) | (1 << B6) | (1 << B7);
	_delay_ms(2);
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

		}
		else if ((B5_PIN & (1 << B5)) == 0)
		{

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

		}
		led_push();
	}
}

void power_down()
{
	PCICR &= ~(1 << PCIE2) & ~(1 << PCIE0); // disable PCINT
	EIMSK |= (1 << INT1); // Enebale INT1 external interrupt on low state
	power_flag = 0;
	led_enable(0);
	SMCR |= (1 << SM1) |(1 << SE);
	sleep_cpu();
}

void power_up()
{
		power_flag = 1;
		SMCR = 0;
		EIMSK &= ~(1 << INT1); //disable INT1
		PCICR |= (1 << PCIE2) | (1 << PCIE0); // enable PCINT
		sei();
		main_mode = 0;
		func_mode = 0;
}
