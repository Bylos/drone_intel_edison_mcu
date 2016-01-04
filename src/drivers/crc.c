/* crc.c
 * Source file for CRC calc
 */

#include "drivers.h"

#define POLYNOMIAL 0xD8
#define TOPBIT (1 << 7)
unsigned char crcTable[256];

void crc_init(void) {
	unsigned char remainder;
	int dividend;
	debug_print(DBG_INFO, "CRC: Building CRC LUT...\t");
	for (dividend = 0; dividend < 256; ++dividend) {
		remainder = dividend;
		unsigned char bit;
		for (bit = 8; bit > 0; --bit) {
			if (remainder & TOPBIT) {
				remainder = (remainder << 1) ^ POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}
		crcTable[dividend] = remainder;
	}
	debug_print(DBG_INFO, "[OK]\n");
}   /* crcInit() */

unsigned char crc_fast(unsigned char const message[], int nBytes) {
	unsigned char data;
	unsigned char remainder = 0;
	int byte;
	for (byte = 0; byte < nBytes; ++byte) {
		data = message[byte] ^ remainder;
		remainder = crcTable[data] ^ (remainder << 8);
	}
	return (remainder);
}   /* crcFast() */
