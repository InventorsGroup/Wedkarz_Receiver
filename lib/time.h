#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../config.h"
#include "button.h"
#include "led.h"

extern volatile unsigned int t1s;
extern volatile unsigned int t10s;

extern volatile int a;
extern volatile int b;

void time_init();

ISR(TIMER0_COMPA_vect);