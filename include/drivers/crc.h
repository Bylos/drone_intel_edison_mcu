/* crc.h
 * Header file for CRC calc
 */

#ifndef __CRC_H
#define __CRC_H

#include "mcu_main.h"

void crc_init(void);
unsigned char crc_fast(unsigned char const message[], int nBytes);

#endif /* __CRC_H */
