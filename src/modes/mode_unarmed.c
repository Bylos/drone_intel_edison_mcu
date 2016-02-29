/*
 * mode_unarmed.c
 *
 *  Created on: 22 déc. 2015
 *      Author: Bylos
 */

#include "modes.h"
#include "drivers.h"

#define CPU_SEND_DURATION	900 // us
#define PWM_DURATION		500 // us

/*******************************************************************************
 * UNARMED MODE
 *******************************************************************************/
void Mode_Unarmed_Init(void) {
	pca_channel_0_3(PCA_ONE_SHOT_1526_HZ_OFFSET, PCA_ONE_SHOT_1526_HZ_OFFSET, PCA_ONE_SHOT_1526_HZ_OFFSET, PCA_ONE_SHOT_1526_HZ_OFFSET);
	lsm_clear_flags();
	xbee_clear_flags();
	cpu_clear_flags();
	debug_print(DBG_INFO, " ---> Entering Unarmed State\n");
}
static unsigned long dbg_cnt = 0;
mcu_mode_t Mode_Unarmed_Run(cpu_mode_t cpu_mode, unsigned long *_time_us) {
	mcu_mode_t next_mode = MCU_MODE_UNARMED;
	/* Check for new sensors values */
	if (lsm_get_inertial_flag()) {
		/* Send to CPU */
		if (cpu_mode != CPU_MODE_UNKNOWN) {
			cpu_send_inertial(lsm_get_inertial_data());
			*_time_us += CPU_SEND_DURATION;
		}
	}
	/* Check for new joystick values */
	if (xbee_get_joystick_flag()) {
		/* Send to CPU */
		if (cpu_mode != CPU_MODE_UNKNOWN) {
			cpu_send_joystick(xbee_get_joystick_data());
			*_time_us += CPU_SEND_DURATION;
		}
	}
	/* Check for new esc values */
	if (cpu_get_esc_pwm_flag()) {
		/* Do nothing but clear data */
		esc_pwm_t esc = cpu_get_esc_pwm_data();
		dbg_cnt++;
		if (!(dbg_cnt%20)) {
			debug_print(DBG_INFO, "%d %d %d %d\n", esc.front_left, esc.front_right, esc.rear_right, esc.rear_left);
		}
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
