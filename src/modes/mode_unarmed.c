/*
 * mode_unarmed.c
 *
 *  Created on: 22 déc. 2015
 *      Author: Bylos
 */

#include "modes.h"
#include "drivers.h"

/*******************************************************************************
 * UNARMED MODE
 *******************************************************************************/
void Mode_Unarmed_Init(void) {
	debug_print(DBG_INFO, " ---> Entering Unarmed State\n");
	pca_stop();
	lsm_clear_flags();
	xbee_clear_flags();
	cpu_clear_flags();
}

mcu_mode_t Mode_Unarmed_Run(cpu_mode_t cpu_mode) {
	mcu_mode_t next_mode = MCU_MODE_UNARMED;
	/* Check for new sensors values */
	if (lsm_get_inertial_flag()) {
		/* Do nothing */
	}
	/* Check for new joystick values */
	if (xbee_get_joystick_flag()) {
		/* Do nothing */
	}
	/* Check for new esc values */
	if (cpu_get_esc_pwm_flag()) {
		/* Do nothing */
	}
	/* Check for mode request */
	if (xbee_get_command_flag()) {
		switch (xbee_get_command()) {
		case RC_COMMAND_ALEXKIDD:
			next_mode = MCU_MODE_ALEXKIDD;
			break;
		default:
			break;
		}
	}
	return next_mode;
}
