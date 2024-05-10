/** @file OpenRailsTrainBrakeState.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum corresponding to the states of TRAIN_BRAKE in Open Rails
 *
 *  ### Contributors
 *  kubicpa3
 */

#pragma once
#include <cstdint>

enum class OpenRailsTrainBrakeState : int8_t {
    QUICK_RELEASE = 0,
    RELEASE = 30,
    NEUTRAL = 60,
    CONT_SERVICE = 80,
    EMERGENCY = 110
};

inline double toDouble(OpenRailsTrainBrakeState brakeState) {
    return static_cast<double>(brakeState) / 100.0;
}