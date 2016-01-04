/* lsm9ds0.h
 * Header file for interfacing LSM9DS0 with the Edison MCU
 */

#ifndef __LSM9DS0_H
#define __LSM9DS0_H

#include "mcu_main.h"

/* LSM9DS0 address on I2C bus */
#define LSM_ADDRESS_G			0x6B	// Gyroscope default address
#define LSM_ADDRESS_XM			0x1D	// Accelerometer and magnetomer default address

/* LSM9DS0 Gyro (G) Registers */
#define LSM_WHO_AM_I_G          0x0F
#define LSM_CTRL_REG1_G         0x20
#define LSM_CTRL_REG2_G         0x21
#define LSM_CTRL_REG3_G         0x22
#define LSM_CTRL_REG4_G         0x23
#define LSM_CTRL_REG5_G         0x24
#define LSM_REFERENCE_G         0x25
#define LSM_STATUS_REG_G        0x27
#define LSM_OUT_X_L_G           0x28
#define LSM_OUT_X_H_G           0x29
#define LSM_OUT_Y_L_G           0x2A
#define LSM_OUT_Y_H_G           0x2B
#define LSM_OUT_Z_L_G           0x2C
#define LSM_OUT_Z_H_G           0x2D
#define LSM_FIFO_CTRL_REG_G     0x2E
#define LSM_FIFO_SRC_REG_G      0x2F
#define LSM_INT1_CFG_G          0x30
#define LSM_INT1_SRC_G          0x31
#define LSM_INT1_THS_XH_G       0x32
#define LSM_INT1_THS_XL_G       0x33
#define LSM_INT1_THS_YH_G       0x34
#define LSM_INT1_THS_YL_G       0x35
#define LSM_INT1_THS_ZH_G       0x36
#define LSM_INT1_THS_ZL_G       0x37
#define LSM_INT1_DURATION_G     0x38

/* LSM9DS0 WHO_AM_I_G Register */
#define LSM_WHOAMI_G_VALUE		0xD4

/* LSM9DS0 Accel/Magneto (XM) Registers */
#define LSM_OUT_TEMP_L_XM       0x05
#define LSM_OUT_TEMP_H_XM       0x06
#define LSM_STATUS_REG_M        0x07
#define LSM_OUT_X_L_M           0x08
#define LSM_OUT_X_H_M           0x09
#define LSM_OUT_Y_L_M           0x0A
#define LSM_OUT_Y_H_M           0x0B
#define LSM_OUT_Z_L_M           0x0C
#define LSM_OUT_Z_H_M           0x0D
#define LSM_WHO_AM_I_XM         0x0F
#define LSM_INT_CTRL_REG_M      0x12
#define LSM_INT_SRC_REG_M       0x13
#define LSM_INT_THS_L_M         0x14
#define LSM_INT_THS_H_M         0x15
#define LSM_OFFSET_X_L_M        0x16
#define LSM_OFFSET_X_H_M        0x17
#define LSM_OFFSET_Y_L_M        0x18
#define LSM_OFFSET_Y_H_M        0x19
#define LSM_OFFSET_Z_L_M        0x1A
#define LSM_OFFSET_Z_H_M        0x1B
#define LSM_REFERENCE_X         0x1C
#define LSM_REFERENCE_Y         0x1D
#define LSM_REFERENCE_Z         0x1E
#define LSM_CTRL_REG0_XM        0x1F
#define LSM_CTRL_REG1_XM        0x20
#define LSM_CTRL_REG2_XM        0x21
#define LSM_CTRL_REG3_XM        0x22
#define LSM_CTRL_REG4_XM        0x23
#define LSM_CTRL_REG5_XM        0x24
#define LSM_CTRL_REG6_XM        0x25
#define LSM_CTRL_REG7_XM        0x26
#define LSM_STATUS_REG_A        0x27
#define LSM_OUT_X_L_A           0x28
#define LSM_OUT_X_H_A           0x29
#define LSM_OUT_Y_L_A           0x2A
#define LSM_OUT_Y_H_A           0x2B
#define LSM_OUT_Z_L_A           0x2C
#define LSM_OUT_Z_H_A           0x2D
#define LSM_FIFO_CTRL_REG       0x2E
#define LSM_FIFO_SRC_REG        0x2F
#define LSM_INT_GEN_1_REG       0x30
#define LSM_INT_GEN_1_SRC       0x31
#define LSM_INT_GEN_1_THS       0x32
#define LSM_INT_GEN_1_DURATION  0x33
#define LSM_INT_GEN_2_REG       0x34
#define LSM_INT_GEN_2_SRC       0x35
#define LSM_INT_GEN_2_THS       0x36
#define LSM_INT_GEN_2_DURATION  0x37
#define LSM_CLICK_CFG           0x38
#define LSM_CLICK_SRC           0x39
#define LSM_CLICK_THS           0x3A
#define LSM_TIME_LIMIT          0x3B
#define LSM_TIME_LATENCY        0x3C
#define LSM_TIME_WINDOW         0x3D
#define LSM_ACT_THS             0x3E
#define LSM_ACT_DUR             0x3F

// accel_scale defines all possible FSR's of the accelerometer:
typedef enum
{
	A_SCALE_2G, // 000: +/- 2g
	A_SCALE_4G, // 001: +/- 4g
	A_SCALE_6G, // 010: +/- 6g
	A_SCALE_8G, // 011: +/- 8g
	A_SCALE_16G // 100: +/- 16g
} accel_scale_t;

// accel_odr defines all possible output data rates of the accelerometer:
typedef enum
{
	A_POWER_DOWN,   // Power-down mode (0x0)
	A_ODR_3_125,    // 3.125 Hz (0x1)
	A_ODR_6_25,     // 6.25 Hz (0x2)
	A_ODR_12_5,     // 12.5 Hz (0x3)
	A_ODR_25,       // 25 Hz (0x4)
	A_ODR_50,       // 50 Hz (0x5)
	A_ODR_100,      // 100 Hz (0x6)
	A_ODR_200,      // 200 Hz (0x7)
	A_ODR_400,      // 400 Hz (0x8)
	A_ODR_800,      // 800 Hz (9)
	A_ODR_1600      // 1600 Hz (0xA)
} accel_odr_t;

// accel_abw defines all possible anti-aliasing filter rates of the accelerometer:
typedef enum
{
	A_ABW_773,		// 773 Hz (0x0)
	A_ABW_194,		// 194 Hz (0x1)
	A_ABW_362,		// 362 Hz (0x2)
	A_ABW_50,		//  50 Hz (0x3)
} accel_abw_t;

// gyro_scale defines the possible full-scale ranges of the gyroscope:
typedef enum
{
	G_SCALE_245DPS,     // 00: +/- 245 degrees per second
	G_SCALE_500DPS,     // 01: +/- 500 dps
	G_SCALE_2000DPS,    // 10: +/- 2000 dps
} gyro_scale_t;

// gyro_odr defines all possible data rate/bandwidth combos of the gyro:
typedef enum
{                           // ODR (Hz) --- Cutoff
	G_ODR_95_BW_12_5 = 0x0, //   95         12.5
	G_ODR_95_BW_25   = 0x1, //   95          25
	// 0x2 and 0x3 define the same data rate and bandwidth
	G_ODR_190_BW_12_5= 0x4, //   190        12.5
	G_ODR_190_BW_25  = 0x5, //   190         25
	G_ODR_190_BW_50  = 0x6, //   190         50
	G_ODR_190_BW_70  = 0x7, //   190         70
	G_ODR_380_BW_20  = 0x8, //   380         20
	G_ODR_380_BW_25  = 0x9, //   380         25
	G_ODR_380_BW_50  = 0xA, //   380         50
	G_ODR_380_BW_100 = 0xB, //   380         100
	G_ODR_760_BW_30  = 0xC, //   760         30
	G_ODR_760_BW_35  = 0xD, //   760         35
	G_ODR_760_BW_50  = 0xE, //   760         50
	G_ODR_760_BW_100 = 0xF, //   760         100
} gyro_odr_t;

// magn_scale defines all possible FSR's of the magnetometer:
typedef enum
{
	M_SCALE_2GS,    // 00: +/- 2Gs
	M_SCALE_4GS,    // 01: +/- 4Gs
	M_SCALE_8GS,    // 10: +/- 8Gs
	M_SCALE_12GS,   // 11: +/- 12Gs
} magn_scale_t;

// magn_odr defines all possible output data rates of the magnetometer:
typedef enum
{
	M_ODR_3_125, // 3.125 Hz (0x00)
	M_ODR_6_25,  // 6.25 Hz (0x01)
	M_ODR_12_5,  // 12.5 Hz (0x02)
	M_ODR_25,   // 25 Hz (0x03)
	M_ODR_50,   // 50 (0x04)
	M_ODR_100,  // 100 Hz (0x05)
} magn_odr_t;

/*
 * Wake up the sensors and set up scales and rates
 */
void lsm_init(void);

/**
 * Retrieve gyroscope's bias
 */
vector_t lsm_gyro_calibrate(void);

/**
 * Set gyroscope bias values
 */
void lsm_gyro_set_bias(vector_t bias);

/**
 * Retrieve accelerometer's bias
 */
vector_t lsm_accel_calibrate(void);

/**
 * Set accelerometer bias values
 */
void lsm_accel_set_bias(vector_t bias);

/*
 * Store accelerometer, magnetometer and gyroscope sensors values
 */
void lsm_read_inertial(void);

inertial_t lsm_get_inertial_data(void);

unsigned char lsm_get_inertial_flag(void);

void lsm_clear_flags(void);


#endif /* __LSM9DS0_H */
