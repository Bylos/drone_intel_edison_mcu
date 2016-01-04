/* cpu.h
 * Header file for interfacing Edison Atom CPU with the Edison MCU
 */

#ifndef __CPU_H
#define __CPU_H

#include "mcu_main.h"

/* list of frame types sent to CPU */
#define MCU_FRAME_JOYSTICK	0x00
#define MCU_FRAME_INERTIAL	0x01
#define MCU_FRAME_COMMAND	0x02
#define MCU_FRAME_EVENT		0x03

/* List of commands sent to CPU */
typedef enum {
	CPU_CMD_1,
	CPU_CMD_2,	// etc.
} cpu_cmd_t;

/* List of events sent to CPU */
typedef enum {
	CPU_EVT_1,
	CPU_EVT_2,	// etc.
} cpu_evt_t;

/* Flags of CPU frames */
#define CPU_FRAME_MODE	1
#define CPU_FRAME_ESC	2

/* List of CPU modes */
typedef enum {
	CPU_MODE_UNKNOWN = -1,
	CPU_MODE_UNARMED = 0,
	CPU_MODE_ARMED = 1,	// etc.
} cpu_mode_t;

void cpu_send_joystick(joystick_t joystick);
void cpu_send_inertial(inertial_t inertial);
void cpu_send_command(cpu_cmd_t command);
void cpu_send_event(cpu_evt_t event);

unsigned char cpu_read_frame(void);
unsigned char cpu_get_mode_flag(void);
unsigned char cpu_get_mode_data(void);
unsigned char cpu_get_esc_pwm_flag(void);
esc_pwm_t cpu_get_esc_pwm_data(void);
void cpu_clear_flags(void);

#endif /* __CPU_H */
