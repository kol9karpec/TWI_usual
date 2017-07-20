#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

#include "bit_operations.h"

#define SDA_DDR DDRD
#define	SDA_PORT PORTD
#define SDA_BIT 1

#define SCL_DDR DDRD
#define SCL_PORT PORTD
#define SCL_BIT 0

#define TWI_FREQ 100000
#define TWI_STATUS_REG (TWSR & 0xf8)

#define TWI_SEND_START (BIT(TWINT) | BIT(TWSTA) | BIT(TWEN))
#define TWI_SEND_TWDR (BIT(TWINT) | BIT(TWEN))
#define TWI_SEND_STOP (BIT(TWINT) | BIT(TWSTO) | BIT(TWEN))
#define TWI_SEND_ACK (BIT(TWINT) | BIT(TWEA) | BIT(TWEN))
#define TWI_SEND_NACK (BIT(TWINT) | BIT(TWEN))


#define SLA_W(addr) ((addr << 1) & 0xfe)
#define SLA_R(addr) ((addr << 1) | 0x01)

#define MAX_ITER_WAIT_TWINT 200

typedef enum twi_status_t {
	FREE = 1,
	START_SENT,
	REP_START_SENT,
	MT,
	MR,
	ERROR_DETECTED,
	NOT_INITIATED
} twi_status_t;

typedef enum twi_error_t {
	NOT_RESPODING = 1,
	NO_ERROR,
	START_SENDING_FAILED,
	SLA_R_NACK_RECEIVED,
	SLA_W_NACK_RECEIVED,
	DATA_NACK_RECEIVED,
	ARBITRATION_LOST,
	BUS_ERROR,
	ANOTHER_ERROR
} twi_error_t;

extern const char * twi_error_str[];

extern volatile twi_error_t twi_error;
extern volatile twi_status_t twi_status; 

void twi_init();
uint8_t twi_send_start();
void twi_send_stop();
uint8_t twi_send_sla_r(uint8_t addr);
uint8_t twi_send_sla_w(uint8_t addr);
uint8_t twi_send_data(uint8_t data);
uint8_t twi_receive_data(uint8_t * dest, uint8_t answer);
uint8_t twi_wait_for_twint(uint16_t max_iter);