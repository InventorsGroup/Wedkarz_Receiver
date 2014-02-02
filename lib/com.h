#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../config.h"

#include "rfm12.h"
#include "led.h"
#include "button.h"

volatile unsigned extern char wait_for_pair;


void parse(uint8_t *data);

void send(uint8_t c);

