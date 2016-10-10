#ifndef _COMMON_UTILS_H_
#define _COMMON_UTILS_H_

#include <stdint.h>

int
hexToBytes(char* s, uint8_t* b, int blen);

void
bytesToHex(uint8_t* b, int blen, char *s);

#endif
