#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "bit_operations.h"

#define LED_BIT 7
#define LED_DDR DDRB
#define LED_PORT PORTB 

#define DSPEED_MODE 0
#define BAUD_RATE(x) (F_CPU/((16/(int)pow(2.,DSPEED_MODE))*(x-1)))

//Buffer
#define DEF_TR_BUFF_SIZE 256
#define DEF_RC_BUFF_SIZE 256

unsigned char TR_BUF[DEF_TR_BUFF_SIZE];
unsigned char RC_BUF[DEF_RC_BUFF_SIZE];

extern volatile uint8_t tr_end;
extern volatile uint8_t rc_end;

extern volatile uint8_t tr_start;
extern volatile uint8_t rc_start;

void push_tr(uint8_t data);
uint8_t pop_tr();
uint8_t tr_is_empty();

uint8_t rc_is_empty();
void push_rc(uint8_t data);
uint8_t pop_rc();

void USART0_init(uint16_t baud_rate);
void USART0_print(const char * format, ...);