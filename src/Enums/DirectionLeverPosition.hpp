#pragma once

#include <cstdint>

/** @file DirectionLevelPosition.h
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of possible positions of the direction lever.
 *
 *  ### Contributors
 *  rehorja8
 */

enum class DirectionLevelPosition : int8_t {
    Neutral = 0,
    Forwards = 1,
    Backwards = -1,
};