/* lsm9ds0.c
 * Source file for interfacing LSM9DS0 with the Edison MCU
 */

#include "drivers.h"

static short gyro_bias_x = 0, gyro_bias_y = 0, gyro_bias_z = 0;
static short accel_bias_x = 0, accel_bias_y = 0, accel_bias_z = 0;
static short gravity_bias = 0;

/**
 * Fast call for single byte write to LSM9DS0 Gyroscope
 *
 */
void lsm_g_write(unsigned char reg, unsigned char value) {
	if(i2c_write(LSM_ADDRESS_G, reg, &value, 1)) {
		debug_print(DBG_ERROR, "write single byte to LSM9DS0 G failed...\n");
	}
}

/**
 * Fast call for single byte read from LSM9DS0 Gyroscope
 *
 */
unsigned char lsm_g_read(unsigned char reg) {
	unsigned char value = 0;
	if(i2c_read(LSM_ADDRESS_G, reg, &value, 1)) {
		debug_print(DBG_ERROR, "read single byte from LSM9DS0 G failed...\n");
	}
	return value;
}

/**
 * Fast call for multiple bytes read from LSM9DS0 Gyroscope
 *
 */
void lsm_g_read_bytes(unsigned char reg, unsigned char *buf, int len) {
	if(i2c_read(LSM_ADDRESS_G, reg | 0x80, buf, len)) {
		debug_print(DBG_ERROR, "read multiple bytes from LSM9DS0 G failed...\n");
	}
}

/**
 * Fast call for single byte write to LSM9DS0 Accelerometer / Magnetometer
 *
 */
void lsm_xm_write(unsigned char reg, unsigned char value) {
	if(i2c_write(LSM_ADDRESS_XM, reg, &value, 1)) {
		debug_print(DBG_ERROR, "write single byte to LSM9DS0 XM failed...\n");
	}
}

/**
 * Fast call for single byte read to LSM9DS0 Accelerometer / Magnetometer
 *
 */
unsigned char lsm_xm_read(unsigned char reg) {
	unsigned char value = 0;
	if(i2c_read(LSM_ADDRESS_XM, reg, &value, 1)) {
		debug_print(DBG_ERROR, "read single byte from LSM9DS0 XM failed...\n");
	}
	return value;
}

/**
 * Fast call for multiple bytes read from LSM9DS0 Accelerometer / Magnetometer
 *
 */
void lsm_xm_read_bytes(unsigned char reg, unsigned char *buf, int len) {
	if(i2c_read(LSM_ADDRESS_XM, reg | 0x80, buf, len)) {
		debug_print(DBG_ERROR, "read multiple bytes from LSM9DS0 XM failed...\n");
	}
}

/**
 * Start and set gyroscope's parameters
 */
void lsm_gyro_start(gyro_scale_t scale, gyro_odr_t odr) {
	debug_print(DBG_INFO, "LSM9DS0: Starting gyroscope...\t");
	/* Setup gyroscope control registers */
	lsm_g_write(LSM_CTRL_REG1_G, 0x0F | (odr << 4));
	lsm_g_write(LSM_CTRL_REG2_G, 0x00);
	lsm_g_write(LSM_CTRL_REG3_G, 0x00);
	lsm_g_write(LSM_CTRL_REG4_G, scale << 4);
	lsm_g_write(LSM_FIFO_CTRL_REG_G, 0x00);
	lsm_g_write(LSM_CTRL_REG5_G, 0x00);
	debug_print(DBG_INFO, "[OK]\n");
}

/**
 * Start and set accelerometer's parameters
 */
void lsm_accel_start(accel_scale_t scale, accel_odr_t odr, accel_abw_t abw) {
	debug_print(DBG_INFO, "LSM9DS0: Starting accelerometer...\t");
	lsm_xm_write(LSM_FIFO_CTRL_REG, 0x00);
	lsm_xm_write(LSM_CTRL_REG0_XM, 0x00);
	lsm_xm_write(LSM_CTRL_REG1_XM, 0x07 | (odr << 4));
	lsm_xm_write(LSM_CTRL_REG2_XM, (abw << 6) | (scale << 3));
	lsm_xm_write(LSM_CTRL_REG3_XM, 0x00);

	/* What is the gravity bias for given scale ? */
	switch (scale) {
	case A_SCALE_2G:
		gravity_bias = 16384;
		break;
	case A_SCALE_4G:
		gravity_bias = 8192;
		break;
	case A_SCALE_6G:
		gravity_bias = 5461; // for 6g the exact value should be 5461.333
		break;
	case A_SCALE_8G:
		gravity_bias = 4096;
		break;
	case A_SCALE_16G:
		gravity_bias = 2048;
		break;
	}
	debug_print(DBG_INFO, "[OK]\n");
}

/**
 * Start and set magnetometer's parameters
 */
void lsm_magn_start(magn_scale_t scale, magn_odr_t odr) {
	debug_print(DBG_INFO, "LSM9DS0: Starting magnetometer...\t");
	lsm_xm_write(LSM_CTRL_REG5_XM, (odr << 2));
	lsm_xm_write(LSM_CTRL_REG6_XM, (scale << 5));
	lsm_xm_write(LSM_CTRL_REG7_XM, 0x00);
	debug_print(DBG_INFO, "[OK]\n");
}

/**
 * Retrieve gyroscope's bias
 */
vector_t lsm_gyro_calibrate(void) {
	vector_t gyro_bias;
	debug_print(DBG_INFO, "LSM9DS0: Calibrating gyroscope...\t");

	/* Setup gyroscope in FIFO mode */
	lsm_g_write(LSM_CTRL_REG5_G, 0x40); // Reboot memory and enable FIFO
	lsm_g_write(LSM_FIFO_CTRL_REG_G, 0x20); // Start FIFO mode
	mcu_sleep(20); // Wait for FIFO filling

	/* Read FIFO and average values */
	int tempx = 0, tempy = 0, tempz = 0;
	short data[3];
	unsigned char sample_num = lsm_g_read(LSM_FIFO_SRC_REG_G) & 0x1F;
	unsigned char i = sample_num;
	while (i--) {
		lsm_g_read_bytes(LSM_OUT_X_L_G, (unsigned char*)data, 6);
		tempx += data[0];
		tempy += data[1];
		tempz += data[2];
	}

	if (sample_num > 0) {
		gyro_bias.x = tempx / sample_num;
		gyro_bias.y = tempy / sample_num;
		gyro_bias.z = tempz / sample_num;
	}

	/* Put back in Bypass mode and disable FIFO */
	lsm_g_write(LSM_FIFO_CTRL_REG_G, 0x00);
	lsm_g_write(LSM_CTRL_REG5_G, 0x00);

	debug_print(DBG_INFO, "[OK]\n");
	return gyro_bias;
}

/**
 * Set gyroscope bias values
 */
void lsm_gyro_set_bias(vector_t bias) {
	gyro_bias_x = bias.x;
	gyro_bias_y = bias.y;
	gyro_bias_z = bias.z;
}

/**
 * Retrieve accelerometer's bias
 */
vector_t lsm_accel_calibrate(void) {
	vector_t accel_bias;
	debug_print(DBG_INFO, "LSM9DS0: Calibrating accelerometer...\t");

	/* Setup accelerometer in FIFO mode */
	lsm_xm_write(LSM_CTRL_REG0_XM, 0x40); // Reboot memory and enable FIFO
	lsm_xm_write(LSM_FIFO_CTRL_REG, 0x20); // Start FIFO mode
	mcu_sleep(20); // Wait for FIFO filling

	/* Read FIFO and average values */
	int tempx = 0, tempy = 0, tempz = 0;
	short data[3];
	unsigned char sample_num = lsm_xm_read(LSM_FIFO_SRC_REG) & 0x1F;
	unsigned char i = sample_num;
	while (i--) {
		lsm_xm_read_bytes(LSM_OUT_X_L_A, (unsigned char*)data, 6);
		tempx += data[0];
		tempy += data[1];
		tempz += data[2] + gravity_bias; // Here we have to correct gravity bias
	}

	if (sample_num > 0) {
		accel_bias.x = tempx / sample_num;
		accel_bias.y = tempy / sample_num;
		accel_bias.z = tempz / sample_num;
	}

	/* Put back in Bypass mode and disable FIFO */
	lsm_xm_write(LSM_FIFO_CTRL_REG, 0x00);
	lsm_xm_write(LSM_CTRL_REG0_XM, 0x00);

	debug_print(DBG_INFO, "[OK]\n");
	return accel_bias;
}

/**
 * Set accelerometer bias values
 */
void lsm_accel_set_bias(vector_t bias) {
	accel_bias_x = bias.x;
	accel_bias_y = bias.y;
	accel_bias_z = bias.z;
}

/*
 * Wake up the sensors and set up scales and rates
 */
void lsm_init() {
	lsm_gyro_start(G_SCALE_500DPS, G_ODR_380_BW_20);
	lsm_accel_start(A_SCALE_4G, A_ODR_400, A_ABW_50);
	lsm_magn_start(M_SCALE_2GS, M_ODR_12_5);

	mcu_sleep(50); // Wait for sensors to wake-up
}

/*
 * Returns accelerometer, magnetometer and gyroscope sensors values
 */
inertial_t lsm_data;
unsigned char lsm_data_flag = 0;
void lsm_read_inertial(void) {

	/* Read Gyroscope values */
	lsm_g_read_bytes(LSM_OUT_X_L_G, (unsigned char*)(&(lsm_data.gyro.x)), 6);
	/* Read Accelerometer values */
	lsm_xm_read_bytes(LSM_OUT_X_L_A, (unsigned char*)(&(lsm_data.accel.x)), 6);
	/* Read magnetometer values */
	lsm_xm_read_bytes(LSM_OUT_X_L_M, (unsigned char*)(&(lsm_data.magn.x)), 6);

	/* Remove offsets */
	lsm_data.gyro.x -= gyro_bias_x;
	lsm_data.gyro.y -= gyro_bias_y;
	lsm_data.gyro.z -= gyro_bias_z;
	lsm_data.accel.x -= accel_bias_x;
	lsm_data.accel.y -= accel_bias_y;
	lsm_data.accel.z -= accel_bias_z;

	/* Correct axes signs */

	/* Raise flag */
	lsm_data_flag = 1;
	return;
}

inertial_t lsm_get_inertial_data(void) {
	lsm_data_flag = 0;
	return lsm_data;
}

unsigned char lsm_get_inertial_flag(void) {
	return lsm_data_flag;
}

void lsm_clear_flags(void) {
	lsm_data_flag = 0;
}
