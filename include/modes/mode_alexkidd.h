/*
 * mode_alexkidd.h
 *
 *  Created on: 22 déc. 2015
 *      Author: Bylos
 */

#ifndef MODE_ALEXKIDD_H_
#define MODE_ALEXKIDD_H_

#include "mcu_main.h"
#include "modes.h"
#include "drivers.h"

void Mode_AlexKidd_Init(void);
mcu_mode_t Mode_AlexKidd_Run(cpu_mode_t cpu_mode);

#endif /* MODE_ALEXKIDD_H_ */
