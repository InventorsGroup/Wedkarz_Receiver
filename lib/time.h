#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../config.h"
#include "button.h"
#include "led.h"
#include "rfm12.h"
#include "com.h"
#include "speaker.h"

uint8_t losuj();

extern volatile unsigned int t1s;
extern volatile unsigned int t10s;
extern volatile char state;

extern volatile int a;
extern volatile int b;
extern volatile int fb_led;

void time_init();

ISR(TIMER0_COMPA_vect);