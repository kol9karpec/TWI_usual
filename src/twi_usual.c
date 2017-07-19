#include "../headers/twi_usual.h"

volatile twi_status_t twi_status = NOT_INITIATED;
volatile twi_error_t twi_error = NO_ERROR;

const char * twi_error_str[] = {
	"Zero error!",
	"Device is not respoding!",
	"No error!",
	"Start sending error!",
	"SLA+R sending failed!",
	"SLA+W sending failed!",
	"Data sending failed!",
	"Data receiving failed!",
	"Illegal SLA+R sending!",
	"Illegal SLA+W sending!",
	"Illegal DATA transmitting!",
	"Illegal DATA receiving!",
	"Illegal START sending!",
	"Illegal STOP sending!",
};

void twi_init() {
	SREG &= ~BIT(7);

	//Setting pullups for SDA, SCL
	SDA_DDR &= ~BIT(SDA_BIT);
	SDA_PORT |= BIT(SDA_BIT);

	SCL_DDR &= ~BIT(SCL_BIT);
	SCL_PORT |= BIT(SCL_BIT);

	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

	twi_status = FREE;

	SREG |= BIT(7);
}

uint8_t twi_send_start() {
	uint8_t result = 1;
	if((twi_status != ERROR_DETECTED) && \
		(twi_status != NOT_INITIATED) && \
		(twi_status != NOT_INITIATED)) {
		TWCR = TWI_SEND_START;
		result = twi_wait_for_twint(MAX_ITER_WAIT_TWINT);
		if(result) {
			if (twi_status == FREE) {
				if (TWI_STATUS_REG == TW_START) {
					twi_status = START_SENT;
				} else {
					twi_status = ERROR_DETECTED;
					twi_error = START_SENDING_FAILED;				
				}
			} else {

			}
		}
	} else {
		twi_status = ERROR_DETECTED;
		twi_error = ILLEGAL_START;
		result = 0;
	}	

	return result;
}

void twi_send_stop() {
	TWCR = TWI_SEND_STOP;
	twi_status = FREE;
	twi_error = NO_ERROR;
}

uint8_t twi_send_sla_r(uint8_t addr) {
	uint8_t result = 1;
	if((twi_status == START_SENT) || \
		(twi_status == REP_START_SENT)) {
		TWDR = SLA_R(addr);
		TWCR = TWI_SEND_TWDR;
		result = twi_wait_for_twint(MAX_ITER_WAIT_TWINT);
		if(result) {
			if(TWI_STATUS_REG != TW_MR_SLA_ACK) {
				twi_status = ERROR_DETECTED;
				twi_error = SLA_R_SENDING_FAILED;
				result = 0;
			} else {
				twi_status = MR;
			}
		}
	} else {
		twi_status = ERROR_DETECTED;
		twi_error = ILLEGAL_SLA_R;
		result = 0;
	}	
	
	return result;
}

uint8_t twi_send_sla_w(uint8_t addr) {
	uint8_t result = 1;
	if((twi_status == START_SENT) || \
		(twi_status == REP_START_SENT)) {
		TWDR = SLA_W(addr);
		TWCR = TWI_SEND_TWDR;
		result = twi_wait_for_twint(MAX_ITER_WAIT_TWINT);
		if(result) {
			if(TWI_STATUS_REG != TW_MT_SLA_ACK) {
				twi_status = ERROR_DETECTED;
				twi_error = SLA_W_SENDING_FAILED;
				result = 0;
			} else {
				twi_status = MT;
			}
		}
	} else {
		twi_status = ERROR_DETECTED;
		twi_error = ILLEGAL_SLA_W;
		result = 0;
	}
	
	return result;
}

uint8_t twi_send_data(uint8_t data) {
	uint8_t result = 1;
	if(twi_status == MT) {
		TWDR = data;
		TWCR = TWI_SEND_TWDR;
		result = twi_wait_for_twint(MAX_ITER_WAIT_TWINT);
		if(result) {
			if(TWI_STATUS_REG != TW_MT_DATA_ACK) {
				twi_status = ERROR_DETECTED;
				twi_error = SLA_W_SENDING_FAILED;
				result = 0;
			}
		}
	} else {
		twi_status = ERROR_DETECTED;
		twi_error = ILLEGAL_DATA_TR;
		result = 0;
	}
	
	return result;
}

uint8_t twi_receive_data(uint8_t * dest, uint8_t answer) {
	uint8_t result = 1;
	if(twi_status == MR) {
		/*if(answer) { //countinue receiving
			TWCR = TWI_SEND_ACK;
		} else {
			TWCR = TWI_SEND_NACK;
		}*/
		TWCR = (answer) ? TWI_SEND_ACK : TWI_SEND_NACK;		
		result = twi_wait_for_twint(MAX_ITER_WAIT_TWINT);
		if(result) {
			if((TWI_STATUS_REG != TW_MR_DATA_ACK) && (answer)) {
				twi_status = ERROR_DETECTED;
				twi_error = DATA_RECEIVE_FAILED;
				result = 0;
			}
		}
	} else {
		twi_status = ERROR_DETECTED;
		twi_error = ILLEGAL_DATA_RC;
		result = 0;
	}
	
	return result;
}

uint8_t twi_wait_for_twint(uint16_t max_iter) {
	uint16_t i = 0;
	uint8_t result = 1;
	while((!(GET_BIT(TWCR,TWINT))) && (i < max_iter) ) {
		i++;
	}

	if(i == max_iter) {
		twi_status = ERROR_DETECTED;
		twi_error = NOT_RESPODING;
		result = 0;
	}

	return result;
}