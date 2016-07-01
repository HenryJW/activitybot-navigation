#include <simpletools.h>

#include "sdcard.h"

static const short D0 = 22, CLK = 23, DI = 24, CS = 25;

int sdcard_mount() {
	return sd_mount(D0, CLK, DI, CS) == 0;
}