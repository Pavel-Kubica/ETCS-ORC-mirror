/** @file TrainBrake.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of states of the train brake in Open Rails
 *
 *  ### Contributors
 *  rehorja8
 */
#pragma once


#include <cstdint>

enum class TrainBrake : int {
    QUICK_RELEASE = 0,
    RELEASE = 30,       // stored as a percentage
    NEUTRAL = 60,       // stored as a percentage
    CONT_SERVICE = 80,  // stored as a percentage
    EMERGENCY = 420     // stored as a percentage
};

/**
 * Converts the TrainBrake enum to a double value that can be POSTed to the Open Rails API.
 * POSTing this value to the Open Rails should result in the train brake of Open Rails train
 * being in the state that the enum describes.
 * @param trainBrake - Train brake to convert.
 * @return double that represents the `trainBrake`.
 */
inline double ToApiRequestDouble(TrainBrake trainBrake) {
    return (static_cast<int>(trainBrake)) / 100.0;
}