#include "./headers/bit_operations.h"
#include "./headers/usart.h"
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

		bh1750_send_command(BH1750_ONE_TIME_H_RES_MODE);
		_delay_ms(180);
		bh1750_read_data(&l_byte, &h_byte);

		result = ((h_byte << 8) | l_byte);
		
		USART0_print("Light: %u\n",result);

		LED_PORT ^= BIT(LED_BIT);
		_delay_ms(250);
	}

	return 0;
}

void init_led() {
	LED_DDR |= BIT(LED_BIT);
	LED_PORT |= BIT(LED_BIT);
}