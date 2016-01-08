/* xbee_zb.h
 * Source file for interfacing XBEE ZB module with EDISON on UART1
 */

#include "drivers.h"

/* Declare rc data variables and associated flags */
joystick_t joystick;
unsigned char joystick_flag = 0;
rc_command_t rc_command;
unsigned char rc_command_flag = 0;

/*
 * Set up UART for communication with module
 */
void xbee_init() {
	debug_print(DBG_INFO, "XBEE ZB: Configuring UART...\t");
	uart_setup(1, 115200);
	gpio_setup(129, 0);	// GPIO 129 is an input
	debug_print(DBG_INFO, "[OK]\n");
}

/*
 * Send state to controller
 */
void xbee_advertize(unsigned char state) {
	unsigned char buffer[6] = {0xFF, 0xFF, 0x02, 0x00, state, 0x00};
	buffer[5] = crc_fast(buffer+3, 2);
	if (uart_write(1, buffer, 6)) {
		debug_print(DBG_ERROR, "XBEE ZB: Error sending state");
	}
}

/*
 * Read UART to get new channel values;
 */
unsigned short xbee_read_frame(void) {
	/* First part : check for RC data on UART */
	short rc_data[16];
	unsigned short rc_channel = 0;
	unsigned char buffer[64];
	unsigned char preamble;
	unsigned char crc;
	rc_t *rc;
	unsigned char i;
	if (gpio_read(129) == 0) return 0;
	uart_read(1, &preamble, 1);
	if (preamble == 0xFF) {
		while (preamble == 0xFF) {
			uart_read(1, &preamble, 1);
		}
		for (i=0; i<preamble; i+=2) {
			uart_read(1, buffer+i, 1);
			uart_read(1, buffer+i+1, 1);
			rc = (rc_t*)(buffer+i);
			rc_data[rc->channel] = rc->data;
			rc_channel |= 1 << rc->channel;
		}
		uart_read(1, &crc, 1);
		if (crc != crc_fast(buffer, preamble)) {
			debug_print(DBG_ERROR, "XBEE ZB: RX CRC\n");
			rc_channel = 0;
		}
	} else {
		debug_print(DBG_ERROR, "XBEE ZB: RX Preamble\n");
	}
	/* Second part : store data and raise flags */
	if (rc_channel & (1 << CH_RIGHT_Y)) { // assume that if throttle is present, yaw pitch and roll are also
		joystick.right.y = rc_data[CH_RIGHT_Y];	// throttle
		joystick.right.x = rc_data[CH_RIGHT_X];	// yaw
		joystick.left.y  = rc_data[CH_LEFT_Y];		// pitch
		joystick.left.x  = rc_data[CH_LEFT_X];		// roll
		joystick_flag = 1;
	}
	if (rc_channel & (1 << CH_COMMAND)) {
		rc_command = rc_data[CH_COMMAND];
		rc_command_flag = 1;
	}
	return rc_channel; // return a non-zero value to reinit timeout
}

joystick_t xbee_get_joystick_data(void) {
	joystick_flag = 0;
	return joystick;
}

unsigned char xbee_get_joystick_flag(void) {
	return joystick_flag;
}

rc_command_t xbee_get_command(void) {
	rc_command_flag = 0;
	return rc_command;
}

unsigned char xbee_get_command_flag(void) {
	return rc_command_flag;
}

void xbee_clear_flags(void) {
	joystick_flag = 0;
	rc_command_flag = 0;
}
