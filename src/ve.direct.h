#ifndef __VE_DIRECT_H__
#define __VE_DIRECT_H__

#include <Arduino.h>

#define CHECKSUM 0x55

namespace ve {
	bool check(uint8_t *payload);

	char *get(uint16_t id, uint8_t flag);

	char *get(uint16_t id);

	void finish(char *payload);
}

#endif // __VE_DIRECT_H__
