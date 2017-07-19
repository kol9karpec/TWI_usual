#include "./headers/bit_operations.h"
#include "./headers/usart.h"
#include "./headers/twi_usual.h"
#include "./headers/bh1750.h"

#define LED_BIT 7
#define LED_DDR DDRB
#define LED_PORT PORTB

void init_led();

int main(void) {
	init_led();

	USART0_init(BAUD_RATE(115200));
	twi_init();

	uint8_t h_byte = 0,
			l_byte = 0;

	uint16_t result = 0;

	while(1) {
		if(twi_send_start()) {
			USART0_print("Success start!\n");
		}
		if(twi_send_sla_w(BH1750_ADDR)) {
			USART0_print("Success sla_w!\n");
		}
		if(twi_send_data(BH1750_ONE_TIME_H_RES_MODE)) {
			USART0_print("Success data!\n");
		}
		twi_send_stop();

		if(twi_send_start()) {
			USART0_print("Success start!\n");
		}
		if(twi_send_sla_r(BH1750_ADDR)) {
			USART0_print("Success sla_r!\n");
		}
		_delay_ms(180);
		if(twi_receive_data(&h_byte,1)) {
			USART0_print("Success receiving!\n");
		}
		if(twi_receive_data(&l_byte,0)) {
			USART0_print("Success receiving!\n");
		}
		twi_send_stop();

		result = ((h_byte << 8) | l_byte);

		USART0_print("Light: %d, %d\nResult: %d\n",h_byte,l_byte,result);

		LED_PORT ^= BIT(LED_BIT);
		_delay_ms(1000);
	}

	return 0;
}

void init_led() {
	LED_DDR |= BIT(LED_BIT);
	LED_PORT |= BIT(LED_BIT);
}