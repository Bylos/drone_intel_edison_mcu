#include "mcu_main.h"
#include "drivers.h"
#include "modes.h"



/* State Machine Variables */
mcu_mode_t mcu_mode = MCU_MODE_BOOT;
mcu_mode_t mcu_next_mode = MCU_MODE_UNARMED;



/*******************************************************************************
 * Send Adverts to Controller
 *******************************************************************************/
#define ADVERT_PERIOD		500 // ms
#define ADVERT_DURATION		500	// us
unsigned long mcu_advert_timer;
unsigned char MCU_Advert(const unsigned long timer) {
	if (timer - mcu_advert_timer >= ADVERT_PERIOD) {
		xbee_advertize(mcu_mode);
		mcu_advert_timer = timer;
		return 1;
	}
	return 0;
}



/*******************************************************************************
 * Get Data from Sensors
 *******************************************************************************/
#define INERTIAL_PERIOD		50	// ms
#define INERTIAL_DURATION	900	// us
unsigned long mcu_inertial_timer;
unsigned char MCU_Get_Inertial_Data(const unsigned long timer) {
	if (timer - mcu_inertial_timer >= INERTIAL_PERIOD) {
		lsm_read_inertial();
		mcu_inertial_timer = timer;
		return 1;
	}
	return 0;
}



/*******************************************************************************
 * Get Data from Controller
 *******************************************************************************/
#define CONTROLLER_TIMEOUT	2000 // ms
#define CONTROLLER_DURATION 3000 // us
unsigned long mcu_controller_timer;
char MCU_Get_Controller_Data(const unsigned long timer) {
	if (xbee_read_frame()) {
		mcu_controller_timer = timer;
		return 1;
	}
	if (timer - mcu_controller_timer >= CONTROLLER_TIMEOUT) {
		mcu_controller_timer = timer;
		debug_print(DBG_WARNING, "Controller RX Timeout\n");
		return -1;
	}
	return 0;
}



/*******************************************************************************
 * Get Data from Processor
 *******************************************************************************/
#define CPU_TIMEOUT		2000 // ms
#define CPU_DURATION	100	 // us
unsigned long mcu_cpu_timer;
char MCU_Get_Processor_Data(const unsigned long timer) {
	if (cpu_read_frame()) {
		mcu_cpu_timer = timer;
		return 1;
	}
	if (timer - mcu_cpu_timer >= CPU_TIMEOUT) {
		mcu_cpu_timer = timer;
		debug_print(DBG_WARNING, "CPU communication Timeout\n");
		return -1;
	}
	return 0;
}

#define CPU_SEND_DURATION	900 // us
#define PWM_DURATION		500 // us
#define LOOP_DURATION		39  // us

unsigned long _time_us = 0;

/*******************************************************************************
 * Main Function
 *******************************************************************************/
void mcu_main()
{
	debug_print(DBG_INFO, "MCU Application Starts\n");

	/* Execution time (ms) */
	unsigned long cur_time = time_ms();

	/* Initialization */
	crc_init();
	pca_init();
	lsm_init();
	xbee_init();
	debug_print(DBG_INFO, "End of initialization %d ms\n", time_ms() - cur_time);

	/* CPU Mode Status */
	cpu_mode_t cpu_mode = CPU_MODE_UNKNOWN;

	/* Debug: log number of frames from peripherals */
	unsigned long cpu_cnt, xbee_cnt, lsm_cnt, adv_cnt, dbg_cnt;
	cpu_cnt = xbee_cnt = lsm_cnt = adv_cnt = 0;
	unsigned long debug_timer = 0;
	/* End of debug */

	/* Infinite loop */
	while (1)
	{
		/* Update execution time */
		cur_time = _time_us / 1000; //time_ms();

		/* Debug: log number of frame from peripherals */
		if (cur_time - debug_timer >= 10000) {
			debug_print(DBG_INFO, "time: %d, cpu: %d, xbee: %d, lsm: %d, adv: %d\n",
					cur_time/1000, cpu_cnt, xbee_cnt, lsm_cnt, adv_cnt);
			cpu_cnt = xbee_cnt = lsm_cnt = adv_cnt = 0;
			debug_timer = cur_time;
		}

		/* Check for new sensors values */
		if (MCU_Get_Inertial_Data(cur_time)) {
			lsm_cnt ++;
			_time_us += INERTIAL_DURATION;
		}

		/* Check for new controller frame */
		switch (MCU_Get_Controller_Data(cur_time)) {
		case -1:
			mcu_next_mode = MCU_MODE_UNARMED;
			break;
		case 1:
			xbee_cnt ++;
			_time_us += CONTROLLER_DURATION;
			break;
		}

		/* Check for new esc pwm values */
		switch (MCU_Get_Processor_Data(cur_time)) {
		case -1:
			cpu_mode = CPU_MODE_UNKNOWN;
			break;
		case 1:
			cpu_cnt ++;
			_time_us += CPU_DURATION;
			if (cpu_get_mode_flag()) {
				cpu_mode = cpu_get_mode_data();
			}
			break;
		}

		/* Send state to controller */
		if (MCU_Advert(cur_time)) {
			adv_cnt ++;
			_time_us += ADVERT_DURATION;
		}

		/* Change and init mode if needed */
		if (mcu_mode != mcu_next_mode) {
			mcu_mode = mcu_next_mode;
			switch (mcu_mode) {
			case MCU_MODE_UNARMED:
				Mode_Unarmed_Init();
				break;
			case MCU_MODE_ALEXKIDD:
				Mode_AlexKidd_Init();
				break;
			default :	// Should not be here anyhow
				mcu_next_mode = MCU_MODE_UNARMED;
				mcu_mode = MCU_MODE_UNARMED;
				Mode_Unarmed_Init();
				break;
			}
			xbee_advertize(mcu_mode);
			_time_us += ADVERT_DURATION;
		}

		/* Execute mode process */
		switch (mcu_mode) {
		case MCU_MODE_UNARMED:
			mcu_next_mode = Mode_Unarmed_Run(cpu_mode, &_time_us);
			break;
		case MCU_MODE_ALEXKIDD:
			mcu_next_mode = Mode_AlexKidd_Run(cpu_mode, &_time_us);
			break;
		default :
			break;
		}

		_time_us += LOOP_DURATION;

		dbg_cnt++;
	}
}
