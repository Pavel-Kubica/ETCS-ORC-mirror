/** @file OpenRailsTrainBrakeState.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum corresponding to possible increments
 *
 *  ### Contributors
 *  kubicpa3
 */

#pragma once
#include <cstdint>

enum class Increment : int8_t
{
    Positive = 1,
    None = 0,
    Negative = -1
};