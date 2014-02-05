volatile unsigned char id_tab[4][3] = {{0,0,0},
									   {0,0,0},
									   {0,0,0},
									   {0,0,0}};
volatile unsigned char id_temp[3]={0,0,0};
volatile unsigned char color[4] ={7, 7, 7, 7};
volatile unsigned char device = 0;
volatile unsigned int bite[4] = {0, 0 ,0 ,0};
volatile unsigned char bite_type[4] = {0, 0, 0, 0};
volatile unsigned char contact[4] = {0, 0, 0, 0};
volatile unsigned int led_counter = 0;
volatile unsigned int func_timer;
volatile char func_mode = 0;
volatile unsigned char rnd[3] = {0,0,0};;