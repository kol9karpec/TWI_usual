#include "../headers/usart.h"

//volatile uint8_t tr_end = 0;
//volatile uint8_t rc_end = 0;

//volatile uint8_t tr_start = 0;
//volatile uint8_t rc_start = 0;

volatile uint8_t tr_end = 0;
volatile uint8_t rc_end = 0;

volatile uint8_t tr_start = 0;
volatile uint8_t rc_start = 0;

uint8_t tr_is_empty() {
  uint8_t result = 0;
  if(tr_start == tr_end) {
    result = 1;
  }
  return result;
}

void push_tr(uint8_t data) {
  if(data != '\0') {
  	TR_BUF[tr_end] = data;
    if(tr_end == (DEF_TR_BUFF_SIZE-1)) {
      tr_end = 0;
    } else {
      tr_end++;
    }    
  }  
}

uint8_t pop_tr() {
  uint8_t result = 0;
  if(tr_is_empty() == 0) {
    result = TR_BUF[tr_start];
    if(tr_start == (DEF_TR_BUFF_SIZE-1)) {
		tr_start = 0;
    }
    else {
    	tr_start++;
    }
  }

  return result;
}

uint8_t rc_is_empty() {
  uint8_t result = 0;
  if(rc_start == rc_end) {
    result = 1;
  }
  return result;
}

void push_rc(uint8_t data) {
  if(data != '\n') {
    if(rc_end == (DEF_RC_BUFF_SIZE-1)) {
      rc_end = 0;
    } else {
      rc_end++;
    }
    RC_BUF[rc_end] = data;
  }  
}

uint8_t pop_rc() {
	uint8_t result = 0;
	if(rc_is_empty() == 0) {
		result = RC_BUF[rc_start];
		if(rc_start == (DEF_RC_BUFF_SIZE-1)) {
			rc_start = 0;
		} else {
			rc_start++;
		}
	}

	return result;
}

void USART0_init(uint16_t baud_rate) {
	SREG &= ~BIT(7);
	//Setting baud rate
	UBRR0 = baud_rate;

	//Enabling double speed mode
	UCSR0A |= (BIT(U2X0) & DSPEED_MODE);

	//Setting up frame format
	//Setting character size to 8
	UCSR0C |= BIT(UCSZ00) | BIT(UCSZ01);

	//Enabling transmitter and receiver
	UCSR0B |= BIT(TXEN0) | BIT (RXEN0) | BIT(RXCIE0);

	SREG |= BIT(7);
}

//Help code block
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
 
static int uart_putchar(char c, FILE *stream)
{
	while(!GET_BIT(UCSR0A,UDRE0)) {}
	UDR0 = c;
	return 0;
}

void USART0_print(const char * format, ...) {
	va_list arg_list; 
	stdout = &mystdout;

	char * result_str = (char*)(malloc(DEF_TR_BUFF_SIZE*sizeof(char)));
	uint8_t _strlen = 0;
	uint8_t i = 0;

	va_start(arg_list,format); //saving list of arguments
	vsnprintf(result_str,DEF_TR_BUFF_SIZE-1,format,arg_list); //generating final string
	
	//printf("Result string: %s",result_str);
	_strlen = strlen(result_str);

	for(i=0 ; i<_strlen ; i++) {
		push_tr((unsigned char)(result_str[i]));
	}

	free(result_str);

	UCSR0B |= BIT(UDRIE0); //turning on USART0 Data Register Empty Interrupt

	va_end(arg_list);
}

ISR(USART0_UDRE_vect) {
	UCSR0B &= ~BIT(UDRIE0);
	if(tr_is_empty() == 0) { //if buffer is not empty
		UDR0 = pop_tr();
		UCSR0B |= BIT(UDRIE0);
	}
}

ISR(USART0_RX_vect) {
	push_rc(UDR0-128);
	//Handling buffer here
}