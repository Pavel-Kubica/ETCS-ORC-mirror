/** @file DirectionLeverPosition.h
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
    Accelerate = 1,
    Continue = 2,
    Neutral = 3,
    ElectrodynamicBrake = 4,
    PneumaticBrake = 5,
    QuickBrake = 6
};