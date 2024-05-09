/** @file DrivingLeverPosition.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of possible positions of the driving lever.
 *
 *  ### Contributors
 *  kubicpa3
 */

#pragma once

#include <cstdint>

enum class DrivingLeverPosition : int8_t {
    Accelerate = 1, // Increase Throttle, disable Engine Brake and Train Brake
    Continue = 2, // Keep Throttle, begin reducing Engine Brake and Train Brake if they were active
    Neutral = 3, // Reduce Throttle, keep all brake states the same (let inertia take its course)
    ElectrodynamicBrake = 4, // Reduce throttle, begin increasing Engine Brake output
    PneumaticBrake = 5, // Reduce throttle, begin increasing Engine Brake and Train Brake output
    QuickBrake = 6 // Emergency state, disable throttle and set all brakes to maximum output
};