#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <stdbool.h>

extern const float MIN_PING_DISTANCE;


typedef enum { 
	NAVIGATION_SPEED_SLOWEST = 26,
	NAVIGATION_SPEED_SLOW = 52,
	NAVIGATION_SPEED_MEDIUM = 78,
	NAVIGATION_SPEED_FAST = 104,
	NAVIGATION_SPEED_FASTEST = 128
} NavigationSpeed;


/*
 * @brief Checks if left is blocked by an obstacle using Ping)))
 * @returns bool True if left is blocked. False otherwise.
 */
bool navigation_is_left_blocked();

/*
 * @brief Checks if right is blocked by an obstacle using Ping)))
 * @returns bool True if right is blocked. False otherwise.
 */
bool navigation_is_right_blocked();

/*
 * @brief Checks if left is blocked by an obstacle using IR flashlight.
 * @returns bool True if left is blocked. False otherwise.
 */
bool navigation_obstacle_detected_by_left_ir();


/*
 * @brief Checks if right is blocked by an obstacle using IR flashlight.
 * @returns bool True if right is blocked. False otherwise.
 */
bool navigation_obstacle_detected_by_right_ir();

/*
 * @brief Checks if front is blocked by an obstacle
 * @returns bool True if front is blocked. False otherwise.
 */
bool navigation_is_front_blocked();

/*
 * @brief Checks if front is direction by an obstacle
 * @param num_degrees Number of degrees where the Ping))) mount will face,
 * with 0 being right, 90 forward, and 180 left.
 * @returns bool True if direction is blocked. False otherwise.
 */
bool navigation_is_direction_blocked(short num_degrees);

/*
 * @brief Returns distance from obstacle at a specified number of degrees for Ping))) mount.
 * @param num_degrees Number of degrees where the Ping))) mount will face,
 * with 0 being right, 90 forward, and 180 left.
 * @returns short Distance, in cm, from the obstacle, if any. If no near obstacle, returns -1.
 */
short navigation_distance_from_obstacle(int num_degrees);

/**
 * @brief Start driving forward at specified speed
 * @param speed Speed at which the robot should drive
 */
void navigation_drive(NavigationSpeed speed);

/**
 * @brief Start driving backwards at specified speed
 * @param speed Speed at which the robot should drive
 */
void navigation_drive_backwards(NavigationSpeed speed);


/**
 * @brief Stop driving
 */
void navigation_stop_driving();


/**
 * @brief Turns the robot left (90 degrees counter clockwise)
 */
void navigation_turn_left();

/**
 * @brief Turns the robot right (90 degrees clockwise)
 */
void navigation_turn_right();

/**
 * @brief Turns the robot slightly left (45 degrees counter clockwise)
 */
void navigation_turn_slightly_left();

/**
 * @brief Turns the robot slightly right (45 degrees clockwise)
 */
void navigation_turn_slightly_right();

/**
 * @brief Move robot forward a bit
 */
void navigation_move_forward();

/**
 * @brief Move robot backwards a bit
 */
void navigation_move_backward();

/**
 * @brief Move robot backwards a bit
 */
void navigation_jump_back();

#endif
