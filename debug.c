#include "debug.h"

void debug_print_message(char * message) {
	if(DEBUG) {
		printf(message);
	}
}