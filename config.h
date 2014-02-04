
  /*
	LED Control PINs, PORTs and DDRs
*/

volatile extern unsigned char id_tab[4][3];
volatile extern unsigned char color[4];
volatile unsigned extern char device;
volatile unsigned extern char id_temp[3];
volatile unsigned extern int bite[4];
volatile unsigned extern char bite_type[4];
volatile unsigned extern char contact[4];
volatile unsigned extern int led_counter;
extern volatile char main_mode;
extern volatile char func_mode;
extern volatile unsigned int func_timer;
volatile extern unsigned char rnd;

//led drivers
#define SDI_PORT PORTC	
#define OE_PORT PORTB
#define CLK_PORT PORTC
#define LA_PORT	PORTC
#define LED_ON_PORT PORTC

#define SDI_DDR	DDRC
#define OE_DDR DDRB
#define CLK_DDR DDRC
#define LA_DDR DDRC
#define LED_ON_DDR DDRC

#define SDI 3
#define OE 1
#define CLK 4
#define LA 5
#define LED_ON 1


//ADC Channels
#define POT1 0
#define POT2 1
#define POT3 4
#define FOTO1 2 // anti -theft
#define FOTO2 3


//Buttons
#define B1_PORT PORTD
#define B2_PORT PORTD
#define B3_PORT PORTB
#define B4_PORT PORTD
#define B5_PORT PORTD
#define B6_PORT PORTD
#define B7_PORT PORTD

#define B1_PIN PIND
#define B2_PIN PIND
#define B3_PIN PINB
#define B4_PIN PIND
#define B5_PIN PIND
#define B6_PIN PIND
#define B7_PIN PIND

#define B1_DDR DDRD
#define B2_DDR DDRD
#define B3_DDR DDRB
#define B4_DDR DDRD
#define B5_DDR DDRD
#define B6_DDR DDRD
#define B7_DDR DDRD

#define B1 3
#define B2 4
#define B3 6
#define B4 6
#define B5 5
#define B6 0
#define B7 1
