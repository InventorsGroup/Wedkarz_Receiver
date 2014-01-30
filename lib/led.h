#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../config.h"


/*
Intializes LEDs
*/

extern unsigned volatile char COLOR;
extern unsigned volatile char BRIGHTNESS;
extern volatile unsigned char led_turn_off;

extern volatile unsigned char led_button_pwr;

void led_brightness_to_power();


void led_init();

/*
Sets the Nth led to the color specified, without turning it on
Color codes:
0 - off, 1 - red, 2 - green, 3 - blue, 4 - yellow, 5 - purple, 6 - sky, 7 - white

Led Numbers:
0-5 - bar leds, 6 - RGB topled (left), 7 - white topled (right), 8 - button led

Annotation:
setting any color >0 on led 7 will turn it on
led 8 takes color for it's brightness
*/
void led_set(unsigned char n,char color);

/*
Turns 0-7 leds off (0) and on (1)
*/
void led_enable(unsigned char s);

/*
Sets % power of the LEDs 0-7
*/
void led_power(unsigned char p);

/*
Sets physical state on leds as preset with led_set function (LEDs 0-7)
*/
void led_push();

/*
Clears LEDs 0-7
*/
void led_clear();
