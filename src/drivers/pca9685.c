/* pca9685.c
 * Source file for interfacing PCA9685 with the Edison MCU
 * Sparkfun's PWM Block only exposes 8 channels so only them
 * are implemented for now.
 * This is a direct implementation of OneShot125 for ESCs
 * connected on channels 0-3
 */

#include "drivers.h"

/**
 * Fast call for single byte write to PCA9685
 */
void pca_write(unsigned char reg, unsigned char buf) {
	if(i2c_write(PCA_ADDRESS, reg, &buf, 1)) {
		debug_print(DBG_ERROR, "write single byte to PCA9685 failed...\n");
	}
}

/**
 * Fast call for multiple bytes write to PCA9685
 */
void pca_write_bytes(unsigned char reg, unsigned char *buf, int len) {
	if(i2c_write(PCA_ADDRESS, reg, buf, len)) {
		debug_print(DBG_ERROR, "write multiple bytes to PCA9685 failed...\n");
	}
}

/**
 * Set up PCA9685 mode and frequency
 */
void pca_init() {
	debug_print(DBG_INFO, "PCA9685: Starting PWM Periods...\t");
	pca_write(PCA_MODE1, 0x00);
	pca_write(PCA_MODE2, 0x00);
	pca_write(PCA_MODE1, PCA_SLEEP);
	pca_write(PCA_PRE_SCALE, PCA_SCALER_1526_HZ);
	pca_write(PCA_MODE1, PCA_RESTART | PCA_AI);
	pca_write(PCA_MODE2, 0x00);
	pca_stop();
	debug_print(DBG_INFO, "[OK]\n");
}

/**
 * Calibrate ESCs to ONESHOT125 mode
 * Must be done just ONCE without previous activations
 */
unsigned char allow_calibration = 1;
void pca_calibrate() {
	if (allow_calibration) {
		debug_print(DBG_INFO, "PCA9685: Calibrating ESCs...\t");
		pca_channel_0_3(
				PCA_ONE_SHOT_1526_HZ_OFFSET + PCA_ONE_SHOT_1526_HZ_RANGE,
				PCA_ONE_SHOT_1526_HZ_OFFSET + PCA_ONE_SHOT_1526_HZ_RANGE,
				PCA_ONE_SHOT_1526_HZ_OFFSET + PCA_ONE_SHOT_1526_HZ_RANGE,
				PCA_ONE_SHOT_1526_HZ_OFFSET + PCA_ONE_SHOT_1526_HZ_RANGE);
		mcu_sleep(500);	// wait 500x 10ms = 5s
		pca_channel_0_3(
					PCA_ONE_SHOT_1526_HZ_OFFSET,
					PCA_ONE_SHOT_1526_HZ_OFFSET,
					PCA_ONE_SHOT_1526_HZ_OFFSET,
					PCA_ONE_SHOT_1526_HZ_OFFSET);
		mcu_sleep(500);
		debug_print(DBG_INFO, "[OK]\n");
	} else {
		debug_print(DBG_ERROR, "PCA9685: Calibration requested is forbidden !\n");
	}
}

/**
 * Stop all channels
 */
void pca_stop() {
	unsigned char stop_bytes[4] = {0x00, 0x00, 0x00, 0x10};
	pca_write_bytes(PCA_ALL_LED, stop_bytes, 4);
}

/**
 * Set channels 0-3 on value in a single sequence
 */
void pca_channel_0_3(int ch0, int ch1, int ch2, int ch3) {
	allow_calibration = 0;
	unsigned char buffer[16] = {
			0x00, 0x00, (unsigned char)(ch0), (unsigned char)(ch0>>8),
			0x00, 0x00, (unsigned char)(ch1), (unsigned char)(ch1>>8),
			0x00, 0x00, (unsigned char)(ch2), (unsigned char)(ch2>>8),
			0x00, 0x00, (unsigned char)(ch3), (unsigned char)(ch3>>8),
	};
	pca_write_bytes(PCA_LED0, buffer, 16);
}
