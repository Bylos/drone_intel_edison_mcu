/*
 * mode_unarmed.h
 *
 *  Created on: 22 déc. 2015
 *      Author: Bylos
 */

#ifndef MODE_UNARMED_H_
#define MODE_UNARMED_H_

#include "mcu_main.h"
#include "modes.h"
#include "drivers.h"

void Mode_Unarmed_Init(void);
mcu_mode_t Mode_Unarmed_Run(cpu_mode_t cpu_mode, unsigned long *_time_us);

#endif /* MODE_UNARMED_H_ */
