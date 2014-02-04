#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../config.h"

#include "rfm12.h"
#include "led.h"

volatile unsigned extern char wait_for_pair;


void parse(uint8_t *data);

<<<<<<< HEAD
void send(uint8_t c, uint8_t dev, uint8_t param);
=======
void send(uint8_t c);
>>>>>>> parent of acd8d37... Fixes

