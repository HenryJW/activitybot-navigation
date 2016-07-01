#ifndef LED_H
#define LED_H
/**
 * @Blinks LED.
 *
 * @param led: pin number for LED
 * @param num_times: number of times to blink LED
 * @param interal: interval in ms between blinks
 */
void led_blink(short led, int num_times, int interval);

#endif
