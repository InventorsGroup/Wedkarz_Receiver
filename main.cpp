#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"
#include "lib/led.h"
#include "lib/pot.h"
#include "lib/speaker.h"
#include "lib/button.h"



int main(void) 
 {  
	led_init();
	button_init();
	sei();

	
	while(1)
	{
		
		
	}
 }

