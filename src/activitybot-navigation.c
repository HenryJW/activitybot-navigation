// Removed all the print calls because the result in weird errors that ultimately
// cause the to not behave as expected

#include <stdlib.h>

#include <simpletools.h>
#include <simpletext.h>

#include "led.h"
#include "navigation.h"
#include "remote_control.h"
#include "settings.h"

#define _BLINK_INTERVAL 300
#define _MIN_IR_DISTANCE_IN_CM 5

#define _DRIVE_SPEED NAVIGATION_SPEED_FAST
#define _DRIVE_SPEED_REVERSE NAVIGATION_SPEED_SLOW

typedef enum { UNBLOCKED, BLOCKED, REVERSING, CHANGED_DIRECTION } DrivingState;

static void _drive();
static void _reverse();
static void _stop_reversing();
static void _let_remote_control_take_over();
static void _let_robot_drive_itself();
static void _face_left();
static void _face_right();
static void _blink_left_led();
static void _blink_right_led();	 

volatile static int *_cog_left_led = NULL;
volatile static int *_cog_right_led = NULL;
volatile static short _is_remote_controlled = 0;

int main(){

	navigation_pin_settings_t navigation_settings = {
		ping_pin_num: PIN_NUM_PING,
		left_ir_receiver_pin_num: PIN_NUM_LEFT_IR_RECEIVER,
		left_ir_flashlight_pin_num: PIN_NUM_LEFT_IR_FLASHLIGHT,
		right_ir_receiver_pin_num: PIN_NUM_RIGHT_IR_RECEIVER,
		right_ir_flashlight_pin_num: PIN_NUM_RIGHT_IR_FLASHLIGHT,
		ping_mount_pin_num: PIN_NUM_PING_MOUNT,
	};

	remote_event_handlers_t remote_event_handlers = {
		on_start: _let_remote_control_take_over,
		on_stop: _let_robot_drive_itself
	};


	remote_button_event_handlers_t remote_button_handlers = {
		on_up_pressed: navigation_move_forward,
		on_down_pressed: navigation_move_backward,
		on_left_pressed: navigation_turn_slightly_left,
		on_right_pressed: navigation_turn_slightly_right,
		on_other_pressed: navigation_stop_driving 
	};

	short remote_control_ir_receiver_pin = PIN_NUM_REMOTE_IR_RECEIVER;
	remote_control_init(remote_event_handlers, remote_button_handlers, remote_control_ir_receiver_pin);
	navigation_init(navigation_settings);

	//Blink to indicate program has started running
    led_blink(26, 3, _BLINK_INTERVAL);
    _drive();
}

void _let_remote_control_take_over() {
	_is_remote_controlled = 1;
	_blink_left_led();
	navigation_stop_driving();
}

void _let_robot_drive_itself() {
	_is_remote_controlled = 0;
	_blink_right_led();
	_drive();
}

void _drive() {
	DrivingState state = UNBLOCKED;
	navigation_drive(_DRIVE_SPEED);

	short is_left_ir_approaching_obstacle = 0;
	short is_right_ir_approaching_obstacle = 0;

	short dist_from_obstacle = 0;
	short is_approaching_obstacle = 0;

	while(!_is_remote_controlled) {
		switch(state) {
		case UNBLOCKED:
			if(navigation_is_front_blocked()) {
				state = BLOCKED;
				break;
			}

			// The logic here is to check using IR flashlight if either side (left or right) is blocked
			// by an obstacle. If so, then we check the distance from the obstacle using the Ping))) sonar.
			// We check twice to see if we are indeed approaching an obstacle since it could be the case that the
			// IR flashlight + receiver is simply detecting an object to the "peripheral" of the robot. If that's the 
			// case, then we simply ignore the "obstacle".
			is_left_ir_approaching_obstacle = navigation_obstacle_detected_by_left_ir();
			is_right_ir_approaching_obstacle = navigation_obstacle_detected_by_right_ir();

			if(is_left_ir_approaching_obstacle) {
				dist_from_obstacle = navigation_distance_from_obstacle(135);
				pause(500);
				is_approaching_obstacle = navigation_distance_from_obstacle(135) < dist_from_obstacle;
			
				if(is_approaching_obstacle || dist_from_obstacle < _MIN_IR_DISTANCE_IN_CM) {
					navigation_stop_driving();
					_face_right();
					state = CHANGED_DIRECTION;

				}

			} else if(is_right_ir_approaching_obstacle) {
				dist_from_obstacle = navigation_distance_from_obstacle(45);
				pause(500);
				is_approaching_obstacle = navigation_distance_from_obstacle(45) < dist_from_obstacle;

				if(is_approaching_obstacle || dist_from_obstacle < _MIN_IR_DISTANCE_IN_CM) {
					navigation_stop_driving();
					_face_left();
					state = CHANGED_DIRECTION;
				}
			}

			break;
		case BLOCKED:
			navigation_jump_back();
			if(!navigation_is_left_blocked()) {
				_face_left();
				state = CHANGED_DIRECTION;
			} else if (!navigation_is_right_blocked()) {
				_face_right();
				state = CHANGED_DIRECTION;
			} else {
				_reverse();
				state = REVERSING;
			}
			break;
		case REVERSING:
			// Don't check forward because otherwise the robot will likely
			// end up in an infinite loop because if the robot is cornered,
			// it will eventually back up enough so its front is clear, which means
			// it'll go forward and get stuck again.
			if(!navigation_is_right_blocked()) {
				_stop_reversing();
				_face_right();
				state = CHANGED_DIRECTION;
			} else if(!navigation_is_left_blocked()) {
				_stop_reversing();
				_face_left();
				state = CHANGED_DIRECTION;
			} else {
				pause(300);
			}
			break;
		case CHANGED_DIRECTION:
			state = UNBLOCKED;
			navigation_drive(_DRIVE_SPEED);
			break;
		default:
			state = BLOCKED;
		}
	}
}

void _face_left() {
	_blink_left_led();
	navigation_turn_left();
}

void _face_right() {
	_blink_right_led();
	navigation_turn_right();
}

void _reverse() {
	_cog_left_led = cog_run(_blink_left_led, 32);
	_cog_right_led = cog_run(_blink_right_led, 32);
	pause(1000);
	navigation_drive_backwards(_DRIVE_SPEED_REVERSE);
}

void _stop_reversing() {
	navigation_stop_driving();
}

void _blink_left_led(void *params) {
	led_blink(PIN_NUM_LEFT_LED, 3, _BLINK_INTERVAL);
	if(_cog_left_led != NULL) {
		cog_end(_cog_left_led);
		_cog_left_led = NULL;
	}
}

void _blink_right_led(void *params) {
	led_blink(PIN_NUM_RIGHT_LED, 3, _BLINK_INTERVAL);
	if(_cog_right_led != NULL) {
		cog_end(_cog_right_led);
		_cog_right_led = NULL;
	}
}