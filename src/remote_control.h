#ifndef CONTROL_REMOTE_H
#define CONTROL_REMOTE_H

/*@brief Initialize the Remote Control module 
 *@param on_start Function pointer to function that will be called when remote control mode is enabled.
 *@param on_stop  Function pointer to function that will be called when remote control mode is disabled.  
 *@param ir_receiver_pin_num  Pin number of the IR receiver that will read remote control input
*/
void remote_control_init(void (*on_start)(void), void (*on_stop)(void), uint16_t ir_receiver_pin_num);

/*@brief Stops remote control module from listening to remote control commands.
*/
void remote_control_stop();

#endif