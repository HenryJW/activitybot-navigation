#include <simpletools.h>

#include "led.h"

void led_blink(short led, int num_times, int interval) {
    for(int i = 1; i <= num_times; i++) {
        high(led);
        pause(interval);
        low(led);
        pause(interval);
    }
}
