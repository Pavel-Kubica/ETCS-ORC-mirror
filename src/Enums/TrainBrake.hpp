/** @file TrainBrake.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of states of the train brake in Open Rails.
 *  To convert this value to a double that can be POSTed to the Open Rails API,
 *  use the OpenRailsTrainBrakeConfiguration class.
 *
 *  ### Contributors
 *  rehorja8
 */
#pragma once


#include <cstdint>

enum class TrainBrake {
    QUICK_RELEASE,
    RELEASE,       // stored as a percentage
    NEUTRAL,       // stored as a percentage
    CONT_SERVICE,  // stored as a percentage
    EMERGENCY      // stored as a percentage
};
