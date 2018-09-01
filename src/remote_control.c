#include <stdbool.h>
	
#include <cog.h>
#include <sirc.h>

#include "navigation.h"
#include "remote_control.h"

#define _SIRC_TIMEOUT 250

typedef enum {
	BUTTON_UP = 116,
	BUTTON_DOWN = 117,
	BUTTON_LEFT = 52,
	BUTTON_RIGHT = 51,
	BUTTON_CENTER = 101,
	BUTTON_POWER = 21
} Button;

static uint16_t _IR_RECEIVER_PIN_NUM;
volatile int *_drive_cog = NULL;
volatile int *_remote_cog_ptr = NULL;
volatile int *_on_stop_cog_ptr = NULL;
volatile int *_on_start_cog_ptr = NULL;
volatile bool _is_listening = 0;
bool _remote_allowed = false;

static void _start();
static void _free_cog(int *cog_ptr);
static void _free_cogs();
static void _handle_button(Button button);

static void (*_on_start)(void);
static void (*_on_stop)(void);


void remote_control_init(void (*on_start)(void), void (*on_stop)(void), uint16_t ir_receiver_pin_num) {
	if(_remote_cog_ptr) _free_cogs();
	sirc_setTimeout(_SIRC_TIMEOUT);
	_remote_cog_ptr = cog_run(_start, 64);
	_remote_allowed = true;
	_is_listening = false;
	_on_start = on_start;
	_on_stop = on_stop;
	_IR_RECEIVER_PIN_NUM = _IR_RECEIVER_PIN_NUM;
  }

void remote_control_stop() {
	_is_listening = false;
	_remote_allowed = false;
	_free_cogs();
}

static void _free_cogs() {
	_free_cog(_remote_cog_ptr);
	_free_cog(_on_start_cog_ptr);
	_free_cog(_on_stop_cog_ptr);
	_free_cog(_drive_cog);
}

static void _free_cog(int *cog_ptr) {
	if(cog_ptr) cog_end(cog_ptr);
	cog_ptr = NULL;
}

static void _start() {
	Button button;
	Button last_button_pressed;
	short num_times_center_pressed = 0;

	while(_remote_allowed) {
		button = sirc_button(_IR_RECEIVER_PIN_NUM);

		bool center_button_was_long_pressed = num_times_center_pressed > 1;

		// Acknowledge center button being pressed
		// only if it's not a long press to prevent the robot from
		// accidentally switching from remote controlled to autonomous
		// or vice versa when the user accidentally holds down the center button
		// while maneuvering the robot.
		if(button == BUTTON_CENTER) {
			num_times_center_pressed++;
			pause(250); //Won't work without this for some reason.
		}		
		else if(button != BUTTON_CENTER && last_button_pressed == BUTTON_CENTER && !center_button_was_long_pressed) {
			_is_listening = !_is_listening;

			if(_is_listening && _on_start) {
				_on_start_cog_ptr = cog_run(_on_start, 64);
				_free_cog(_on_stop_cog_ptr);
			}
			else if(!_is_listening && _on_stop) {
				_on_stop_cog_ptr = cog_run(_on_stop, 64);
				_free_cog(_on_start_cog_ptr);
			}
		} else {
			num_times_center_pressed = 0;
		}
		if(_is_listening && button != BUTTON_CENTER) _handle_button(button);

		last_button_pressed = button;
	}
}

static void _handle_button(Button button) {
	void (*move_function)(void *) = NULL;
	switch(button) {
		case BUTTON_UP:
			 move_function = navigation_move_forward;
			break;
		case BUTTON_DOWN:
			move_function = navigation_move_backward;
			break;
		case BUTTON_LEFT:
			move_function = navigation_turn_slightly_left;
			break;
		case BUTTON_RIGHT:
			move_function = navigation_turn_slightly_right;
			break;
		default: 
			move_function = navigation_stop_driving;
	}

	if(_drive_cog) _free_cog(_drive_cog);

	if(move_function) _drive_cog = cog_run(move_function, 32);
}