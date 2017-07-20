#include "../headers/bh1750.h"

uint8_t bh1750_send_command(uint8_t opecode) {
	uint8_t result = 1;
	result = twi_send_start();
	if(result) {
		result = twi_send_sla_w(BH1750_ADDR_L);
	} else {
		//Error handling
	}
	if(result) {
		result = twi_send_data(opecode);
	} else {
		//Error handling
	}
	twi_send_stop();
	
	return result;
}

uint8_t bh1750_change_mtreg(uint8_t new_mtreg) {
	uint8_t result = 1;

	result = bh1750_send_command(BH1750_CHANGE_MTREG_H(new_mtreg));
	result = bh1750_send_command(BH1750_CHANGE_MTREG_L(new_mtreg));
	
	return result;
}

uint8_t bh1750_read_data(uint8_t * l_byte_dest, 
						uint8_t * h_byte_dest) {
	uint8_t result = 1;
	result = twi_send_start();
	if(result) {
		result = twi_send_sla_r(BH1750_ADDR_L);
	}
	if(result) {
		result = twi_receive_data(h_byte_dest,1);
	}
	if(result) {
		result = twi_receive_data(l_byte_dest,0);
	}
	twi_send_stop();

	return result;
}

