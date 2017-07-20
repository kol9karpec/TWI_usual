#include "../headers/bh1750.h"

uint8_t bh1750_send_command(uint8_t opecode) {
	uint8_t result = 1;
	result = twi_send_start();
	if(result) {
		result = twi_send_sla_w(BH1750_ADDR_L);
	} else {
		//TODO: Error handling
	}
	if(result) {
		result = twi_send_data(opecode);
	} else {
		//TODO: Error handling
	}
	if(result) {
		twi_send_stop();
	} else {
		//TODO: Error handling
	}
	
	return result;
}

uint8_t bh1750_change_mtreg(uint8_t new_mtreg) {
	//TODO: Implementation
}

uint8_t bh1750_read_data(uint8_t * l_byte_dest, 
						uint8_t * h_byte_dest) {
	//TODO: Implementation
}