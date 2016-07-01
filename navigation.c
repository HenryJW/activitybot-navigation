#include <stdbool.h>

#include <abdrive.h>
#include <ping.h>
#include <servo.h>

#include "navigation.h"
#include "settings.h"

#define _TIME_BETWEEN_MOVEMENTS 500
#define _NUM_TICKS_FOR_TURN 26
#define _NUM_TICKS_SMALL_TURN 13
#define _NUM_TICKS_FULL_ROTATION 52
#define _NUM_TICKS_SMALL_ROTATION 26

static void _ping_mount_face_forward();
static void _ping_mount_face_direction(int direction);
static void _drive(NavigationSpeed speed, bool forward);

const float MIN_PING_DISTANCE = 2.56 * 12;

static bool _is_driving = false;

bool navigation_is_left_blocked() {
	return navigation_is_direction_blocked(180);
}

bool navigation_is_right_blocked() {
	return navigation_is_direction_blocked(0);
}

bool navigation_is_front_blocked() {
	_ping_mount_face_forward();
	return ping_cm(PING_PIN_NUM) < MIN_PING_DISTANCE;
}

bool navigation_is_direction_blocked(short num_degrees) {
	return navigation_distance_from_obstacle(num_degrees) < MIN_PING_DISTANCE;
}

bool navigation_obstacle_detected_by_left_ir() {
	freqout(LEFT_IR_FLASHLIGHT_PIN_NUM, 1, 38000);
	return input(LEFT_IR_RECEIVER_PIN_NUM) == 0;
}

bool navigation_obstacle_detected_by_right_ir() {
	freqout(RIGHT_IR_FLASHLIGHT_PIN_NUM, 1, 38000);
	return input(RIGHT_IR_RECEIVER_PIN_NUM) == 0;
}

void navigation_drive(NavigationSpeed speed) {
	_drive(speed, true);
}

void navigation_drive_backwards(NavigationSpeed speed) {
	_drive(speed, false);
}

void navigation_stop_driving() {
	drive_ramp(0, 0);
	_is_driving = false;
}

void navigation_move_forward() {
	drive_goto(_NUM_TICKS_FULL_ROTATION, _NUM_TICKS_FULL_ROTATION);
}

void navigation_move_backward() {
	drive_goto(-_NUM_TICKS_FULL_ROTATION , -_NUM_TICKS_FULL_ROTATION);
}

void navigation_turn_left() {
	drive_goto(-_NUM_TICKS_FOR_TURN, _NUM_TICKS_FOR_TURN);
}

void navigation_turn_slightly_left() {
	drive_goto(-_NUM_TICKS_SMALL_TURN, _NUM_TICKS_SMALL_TURN);
}

void navigation_turn_right() {
	drive_goto(_NUM_TICKS_FOR_TURN, -_NUM_TICKS_FOR_TURN);
}

void navigation_turn_slightly_right() {
	drive_goto(_NUM_TICKS_SMALL_TURN, -_NUM_TICKS_SMALL_TURN);
}

short navigation_distance_from_obstacle(int num_degrees) {
	_ping_mount_face_direction(num_degrees * 10);
	int distance = ping_cm(PING_PIN_NUM);
	_ping_mount_face_forward();

	return distance;
}

void navigation_jump_back() {
	if(_is_driving) navigation_stop_driving();
	drive_goto(-_NUM_TICKS_SMALL_ROTATION, -_NUM_TICKS_SMALL_ROTATION);
}


static void _drive(NavigationSpeed speed, bool forward) {
	if(_is_driving) {
		navigation_stop_driving();
		_drive(speed, forward);
		return;
	}

	short driving_speed;
	switch(speed) {
		case NAVIGATION_SPEED_SLOWEST:
			driving_speed = NAVIGATION_SPEED_SLOWEST;
			break;
		case NAVIGATION_SPEED_SLOW:
			driving_speed = NAVIGATION_SPEED_SLOW;
			break;
		case NAVIGATION_SPEED_MEDIUM:
			driving_speed = NAVIGATION_SPEED_MEDIUM;
			break;
		case NAVIGATION_SPEED_FAST:
			driving_speed = NAVIGATION_SPEED_FAST;
			break;
		case NAVIGATION_SPEED_FASTEST:
			driving_speed = NAVIGATION_SPEED_FASTEST;
			break;
		default:
			driving_speed = NAVIGATION_SPEED_MEDIUM;
	}

	if(forward) {
		drive_ramp(driving_speed, driving_speed);
	} else {
		drive_ramp(-driving_speed, -driving_speed);
	}

	_is_driving = true;
}

static void _ping_mount_face_forward() {
    _ping_mount_face_direction(900);
}

static void _ping_mount_face_direction(int direction) {
    servo_angle(PING_MOUNT_PIN_NUM, direction);
    pause(_TIME_BETWEEN_MOVEMENTS);
}