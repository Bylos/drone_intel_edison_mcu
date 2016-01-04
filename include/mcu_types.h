/*
 * types.h
 *
 *  Created on: 21 déc. 2015
 *      Author: Bylos
 */

#ifndef __MCU_TYPES_H
#define __MCU_TYPES_H

/* Standard types INTEL EDISON MCU IS LSB FIRST */
typedef signed char		int8_t;
typedef unsigned char	uint8_t;
typedef signed short	int16_t;
typedef unsigned short	uint16_t;
typedef signed long		int32_t;
typedef unsigned long	uint32_t;

typedef enum {
	MCU_MODE_BOOT		= -1,
	MCU_MODE_UNARMED	= 0,
	MCU_MODE_ALEXKIDD	= 1,
} mcu_mode_t;

/* Convenient structure types for inertial data, size is 18 bytes */
typedef struct {
	short x;
	short y;
	short z;
} vector_t;

typedef struct {
	vector_t gyro;
	vector_t accel;
	vector_t magn;
} inertial_t;

/* Convenient structure types for esc pwm data, size is 8 bytes */
typedef struct {
	unsigned short front_left;
	unsigned short front_right;
	unsigned short rear_right;
	unsigned short rear_left;
} esc_pwm_t;

/* Convenient structure types for joystick data, size is 8 bytes */
typedef struct {
	signed short x;
	signed short y;
} axe_t;

typedef struct {
	axe_t left;
	axe_t right;
} joystick_t;

#endif /* __MCU_TYPES_H */
