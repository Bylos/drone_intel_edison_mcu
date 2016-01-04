/* xbee_zb.h
 * Header file for interfacing XBEE ZB module with EDISON on UART1
 */

#ifndef __XBEE_ZB_H
#define __XBEE_ZB_H

#include "mcu_main.h"
#include "crc.h"

#define RC_CHANNEL_NUM	6
#define RC_CHANNEL_MASK	0xF000
#define RC_CHANNEL_SHIFT 12

typedef enum {
	CH_COMMAND			= 0,	// command
	CH_RIGHT_Y			= 1,	// throttle
	CH_RIGHT_X			= 2,	// yaw
	CH_LEFT_Y			= 3,	// pitch
	CH_LEFT_X			= 4,	// roll
} rc_channel_t;

/* Convenient structure for RC data */
typedef struct {
	signed short data :12;
	unsigned short channel  :4;
} rc_t;

typedef enum {
	RC_COMMAND_UNARM	= 0,
	RC_COMMAND_ALEXKIDD = 1,
} rc_command_t;

/*
 * Set up UART for communication with module
 */
void xbee_init();

/*
 * Read UART to get new channel values;
 * return 0 if no new data from controller
 */
unsigned short xbee_read_frame(void);

/*
 * Send Advertisement
 */
void xbee_advertize(unsigned char state);

/*
 * Check and low new joystick data flag
 */
unsigned char xbee_get_joystick_flag(void);

/*
 * Get last joystick data read
 */
joystick_t xbee_get_joystick_data(void);

unsigned char xbee_get_command_flag(void);

rc_command_t xbee_get_command(void);

void xbee_clear_flags(void);

#endif /* __XBEE_ZB_H */
