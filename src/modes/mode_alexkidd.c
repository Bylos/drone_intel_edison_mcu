/*
 * mode_alexkidd.c
 *
 *  Created on: 22 déc. 2015
 *      Author: Bylos
 */

#include "modes.h"
#include "drivers.h"

/*******************************************************************************
 * ALEXKID MODE
 *******************************************************************************/
void Mode_AlexKidd_Init(void) {
	debug_print(DBG_INFO, " ---> Entering Alex Kidd State\n");
}

mcu_mode_t Mode_AlexKidd_Run(cpu_mode_t cpu_mode) {
	mcu_mode_t next_mode = MCU_MODE_ALEXKIDD;
	/* Check for new sensors values */
	if (lsm_get_inertial_flag()) {
		/* Do nothing */
	}
	/* Check for new joystick values */
	if (xbee_get_joystick_flag()) {
		/* Get them */
		joystick_t joystick = xbee_get_joystick_data();
		/* Then play with motors */
		int front_diff  = ((int)(joystick.left.y)  * 27) >> (6+3); // pitch limited to 12.5%
		int sides_diff  = ((int)(joystick.left.x)  * 27) >> (6+3); // roll limited to 12.5%
		int pwm[4] = {
				front_diff - sides_diff,	// front left motor
				front_diff + sides_diff,	// front right motor
				- front_diff + sides_diff,  // rear right motor
				- front_diff - sides_diff	// rear left motor
		};
		/* Just verify pwm range and limit max to 25% per motor */
		pwm[0] = pwm[0] < 0 ? 0 : pwm[0];
		pwm[0] = pwm[0] > (PCA_ONE_SHOT_1526_HZ_RANGE >> 2) ? PCA_ONE_SHOT_1526_HZ_RANGE >> 2 : pwm[0];
		pwm[1] = pwm[1] < 0 ? 0 : pwm[1];
		pwm[1] = pwm[1] > (PCA_ONE_SHOT_1526_HZ_RANGE >> 2) ? PCA_ONE_SHOT_1526_HZ_RANGE >> 2 : pwm[1];
		pwm[2] = pwm[2] < 0 ? 0 : pwm[2];
		pwm[2] = pwm[2] > (PCA_ONE_SHOT_1526_HZ_RANGE >> 2) ? PCA_ONE_SHOT_1526_HZ_RANGE >> 2 : pwm[2];
		pwm[3] = pwm[3] < 0 ? 0 : pwm[3];
		pwm[3] = pwm[3] > (PCA_ONE_SHOT_1526_HZ_RANGE >> 2) ? PCA_ONE_SHOT_1526_HZ_RANGE >> 2 : pwm[3];
		/* Then put that to pwm driver ! */
		pca_channel_0_3(
				pwm[0] + PCA_ONE_SHOT_1526_HZ_OFFSET,
				pwm[1] + PCA_ONE_SHOT_1526_HZ_OFFSET,
				pwm[2] + PCA_ONE_SHOT_1526_HZ_OFFSET,
				pwm[3] + PCA_ONE_SHOT_1526_HZ_OFFSET);
	}
	/* Check for new esc values */
	if (cpu_get_esc_pwm_flag()) {
		/* Do nothing */
	}
	/* Check for mode request */
	if (xbee_get_command_flag()) {
		switch (xbee_get_command()) {
		case RC_COMMAND_UNARM:
			next_mode = MCU_MODE_UNARMED;
			break;
		default:
			break;
		}
	}
	return next_mode;
}
