#ifndef MECHANICAL_TESTER_H
#define MECHANICAL_TESTER_H

// Optional includes if the function uses types from other headers

#include <stdio.h>  // Example
#include <stdint.h> // For fixed-width integers
#include <stdbool.h>

#include "ip_connection.h"
#include "brick_silent_stepper.h"

// Checks if motor has remaining steps. No solution found 
/**
 * @brief Checks if the stepper motor is moving.
 * 
 * @param param1 Silent stepper.
 * @return True if moving, False if not moving
 */
bool silent_stepper_is_moving(SilentStepper *silent_stepper);



/**
 * @brief Moves the motor to a target position while logging the current position, velocity and remaining steps.
 * 
 * @param param1 Silent stepper.
 * @param param2 Target position.
 * 
 */
void silent_stepper_target_info(SilentStepper *silent_stepper, int32_t target_pos);


/**
 * @brief Moves the motor back to zero position while logging the current position, velocity and remaining steps.
 * 
 * @param param1 Silent stepper.
 */
void silent_stepper_zero_info(SilentStepper *silent_stepper);

#endif // MECHANICAL_TESTER_H
