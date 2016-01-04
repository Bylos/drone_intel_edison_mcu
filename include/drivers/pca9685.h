/* pca9685.h
 * Header file for interfacing PCA9685 with the Edison MCU
 * Sparkfun's PWM Block only exposes 8 channels so only them
 * are implemented for now.
 * This is a direct implementation of OneShot125 for ESCs
 * connected on channels 0-3
 */

#ifndef __PCA9685_H
#define __PCA9685_H

#include "mcu_main.h"

/* PCA9685 address on I2C bus */
#define PCA_ADDRESS	0x40

/* PCA9685 Registers Addresses */
#define PCA_MODE1           0x00    // Mode  register  1
#define PCA_MODE2           0x01    // Mode  register  2
#define PCA_SUBADR1         0x02	// I2C-bus subaddress 1
#define PCA_SUBADR2         0x03	// I2C-bus subaddress 2
#define PCA_SUBADR3         0x04	// I2C-bus subaddress 3
#define PCA_ALLCALLADR      0x05    // LED All Call I2C-bus address
#define PCA_LED0            0x06 	// LED0 start register
#define PCA_LED0_ON_L       0x06	// LED0 output and brightness control byte 0
#define PCA_LED0_ON_H       0x07	// LED0 output and brightness control byte 1
#define PCA_LED0_OFF_L      0x08	// LED0 output and brightness control byte 2
#define PCA_LED0_OFF_H      0x09	// LED0 output and brightness control byte 3
#define PCA_LED1_ON_L       0x06    // LED0 output and brightness control byte 0
#define PCA_LED1_ON_H       0x07    // LED0 output and brightness control byte 1
#define PCA_LED1_OFF_L      0x08    // LED0 output and brightness control byte 2
#define PCA_LED1_OFF_H      0x09    // LED0 output and brightness control byte 3
#define PCA_LED2_ON_L       0x06    // LED0 output and brightness control byte 0
#define PCA_LED2_ON_H       0x07    // LED0 output and brightness control byte 1
#define PCA_LED2_OFF_L      0x08    // LED0 output and brightness control byte 2
#define PCA_LED2_OFF_H      0x09    // LED0 output and brightness control byte 3
#define PCA_LED3_ON_L       0x06    // LED0 output and brightness control byte 0
#define PCA_LED3_ON_H       0x07    // LED0 output and brightness control byte 1
#define PCA_LED3_OFF_L      0x08    // LED0 output and brightness control byte 2
#define PCA_LED3_OFF_H      0x09    // LED0 output and brightness control byte 3
#define PCA_ALL_LED		    0xFA    // ALL_LED start register
#define PCA_ALL_LED_ON_L    0xFA    // load all the LEDn_ON registers, byte 0 (turn 0-7 channels on)
#define PCA_ALL_LED_ON_H    0xFB	// load all the LEDn_ON registers, byte 1 (turn 8-15 channels on)
#define PCA_ALL_LED_OFF_L   0xFC	// load all the LEDn_OFF registers, byte 0 (turn 0-7 channels off)
#define PCA_AL_LLED_OFF_H   0xFD	// load all the LEDn_OFF registers, byte 1 (turn 8-15 channels off)
#define PCA_PRE_SCALE       0xFE	// prescaler for output frequency

/* PCA_MODE_0 Register constants */
#define PCA_RESTART			0x80
#define PCA_EXTCLK			0x40
#define PCA_AI				0x20
#define PCA_SLEEP			0x10
#define PCA_SUB1			0x08
#define PCA_SUB2			0x04
#define PCA_SUB3			0x02
#define PCA_ALLCALL			0x01

/* PCA_MODE_1 Register constants */
#define PCA_INVRT			0x10
#define PCA_OCH				0x08
#define PCA_OUTDRV			0x04
#define PCA_OUT_OE_OFF		0x00
#define PCA_OUT_OE_DRV		0x01
#define PCA_OUT_OE_HIGH		0x02

/* PCA_PRE_SCALE Register constants */
#define PCA_SCALER_1526_HZ	0x03

/* PWM constant for OneShot125 Mode at 1526 Hz */
#define PCA_ONE_SHOT_1526_HZ_OFFSET		0x0360	// 125us is 864 PWM value 864 = 27*2^5
#define PCA_ONE_SHOT_1526_HZ_RANGE		0x0360

/**
 * Fast call for single byte write to PCA9685
 */
void pca_write(unsigned char reg, unsigned char buf);

/**
 * Fast call for multiple bytes write to PCA9685
 */
void pca_write_bytes(unsigned char reg, unsigned char *buf, int len);

/**
 * Set up PCA9685 mode and frequency
 */
void pca_init();

/**
 * Calibrate ESCs to ONESHOT125 mode
 * Must be done just after init
 */
void pca_calibrate();

/**
 * Stop all channels
 */
void pca_stop();

/**
 * Set channels 0-3 on value in a single sequence
 */
void pca_channel_0_3(int ch0, int ch1, int ch2, int ch3);

#endif /* __PCA9685_H */
