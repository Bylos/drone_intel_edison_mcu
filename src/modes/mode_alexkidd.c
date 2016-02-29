/*
 * mode_alexkidd.c
 *
 *  Created on: 22 déc. 2015
 *      Author: Bylos
 */

#include "modes.h"
#include "drivers.h"

#define CPU_SEND_DURATION	900 // us
#define PWM_DURATION		500 // us

/*******************************************************************************
 * ALEXKID MODE
 *******************************************************************************/
void Mode_AlexKidd_Init(void) {
	debug_print(DBG_INFO, " ---> Entering Alex Kidd State\n");
}

mcu_mode_t Mode_AlexKidd_Run(cpu_mode_t cpu_mode, unsigned long *_time_us) {
	mcu_mode_t next_mode = MCU_MODE_ALEXKIDD;
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
		// Get them
		joystick_t joystick = xbee_get_joystick_data();
		/* Send to CPU */
		if (cpu_mode != CPU_MODE_UNKNOWN) {
			cpu_send_joystick(xbee_get_joystick_data());
			*_time_us += CPU_SEND_DURATION;
		}
		// Then play with motors
		int front_diff  = (joystick.left.y  * 27) >> (6+3); // pitch limited to 12.5%
		int sides_diff  = (joystick.left.x  * 27) >> (6+3); // roll limited to 12.5%
		int pwm[4] = {
				front_diff - sides_diff,	// front left motor
				front_diff + sides_diff,	// front right motor
				- front_diff + sides_diff,  // rear right motor
				- front_diff - sides_diff	// rear left motor
		};
		// Just verify pwm range and limit max to 25% per motor
		int pwm_max = PCA_ONE_SHOT_1526_HZ_RANGE >> 2;
		if(pwm[0] < 0) pwm[0] = 0;
		if(pwm[0] > pwm_max) pwm[0] = pwm_max;
		if(pwm[1] < 0) pwm[1] = 0;
		if(pwm[1] > pwm_max) pwm[1] = pwm_max;
		if(pwm[2] < 0) pwm[2] = 0;
		if(pwm[2] > pwm_max) pwm[2] = pwm_max;
		if(pwm[3] < 0) pwm[3] = 0;
		if(pwm[3] > pwm_max) pwm[3] = pwm_max;
		// Then put that to pwm driver !
		pca_channel_0_3(
				pwm[0] + PCA_ONE_SHOT_1526_HZ_OFFSET,
				pwm[1] + PCA_ONE_SHOT_1526_HZ_OFFSET,
				pwm[2] + PCA_ONE_SHOT_1526_HZ_OFFSET,
				pwm[3] + PCA_ONE_SHOT_1526_HZ_OFFSET);
		*_time_us += PWM_DURATION;

		/* Send to CPU */
		if (cpu_mode != CPU_MODE_UNKNOWN) {
			cpu_send_inertial(lsm_get_inertial_data());
			*_time_us += CPU_SEND_DURATION;
		}
	}
	/* Check for new esc values */
	if (cpu_get_esc_pwm_flag()) {
		/* Do nothing but clear data */
		esc_pwm_t pwm = cpu_get_esc_pwm_data();
		pca_channel_0_3(
						pwm.front_left + PCA_ONE_SHOT_1526_HZ_OFFSET,
						pwm.front_right + PCA_ONE_SHOT_1526_HZ_OFFSET,
						pwm.rear_right + PCA_ONE_SHOT_1526_HZ_OFFSET,
						pwm.rear_left + PCA_ONE_SHOT_1526_HZ_OFFSET);
		*_time_us += PWM_DURATION;
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
