#include "../config.h"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "led.h"



void button_init();

ISR(INT1_vect);
ISR(PCINT0_vect);
ISR(PCINT2_vect);