#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "../config.h"
#include "led.h"
#include "com.h"

volatile extern unsigned char power_btn;
volatile extern char func_btn;
extern volatile int power_flag;
extern volatile char main_mode;

volatile extern char function;




void button_init();

ISR(INT1_vect);
ISR(PCINT0_vect);
ISR(PCINT2_vect);

void power_down();
void power_up();