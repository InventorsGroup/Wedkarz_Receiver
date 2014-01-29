#include "button.h"


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
	
	EIMSK |= (1 << INT1); // Enebale INT1 external interrupt on low state
	PCICR |= (1 << PCIE2) | (1 << PCIE1) | (1 << PCIE0); // Enable Pin change interrupts
	//Select triggers for PCINTs
	PCMSK0 |= (1 << PCINT6);
	PCMSK2 |= (1 << PCINT22) | (1 << PCINT21) | (1 << PCINT20) | (1 << PCINT17) | (1 << PCINT16);
	
}

ISR(INT1_vect) //TOP (Power Button) INT
{
	
	led_set(0,1);
	led_push();
	_delay_ms(50);
	led_clear();
	while (!(B1_PIN & (1 << B1)));
	_delay_ms(10);
}

ISR(PCINT0_vect)
{
	char mask = (1 << B3);
	_delay_ms(10);
	if ((PINB & mask) != mask)
	{
		led_set(0,2);
		led_push();
		_delay_ms(50);
		led_clear();
	}
}

ISR(PCINT2_vect)
{
	
	int i = 1;

	char mask = 0;
	
	mask |= (1 << B2) | (1 << B4) | (1 << B5) | (1 << B6) | (1 << B7);
	_delay_ms(2);
	if ((PIND & mask) != mask)
	{
		if ((B4_PIN & (1 << B4)) == 0)
		{	
			i = 3;
		}
		else if ((B5_PIN & (1 << B5)) == 0)
		{
			i = 4;
		}
		
		else if ((B6_PIN & (1 << B6)) == 0)
		{
			i = 5;
		}	
		else if ((B7_PIN & (1 << B7)) == 0)
		{
			i = 6;
		}	
		else if ((B2_PIN & (1 << B2)) == 0)
		{
			i = 7;
		}
		led_set(0,i);
		led_push();
		_delay_ms(50);
		led_clear();
	}
}
