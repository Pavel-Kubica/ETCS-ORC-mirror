#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

/** @file DirectionLeverPosition.h
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

enum class DirectionLeverPosition : int8_t {
    Neutral = 0,
    Forwards = 1,
    Backwards = -1,
};

/**
 * Conversion methods to/from double.
 * Used for communication with OpenRails API.
*/
namespace DirectionLeverPositionMethods {
    inline DirectionLeverPosition FromDouble(double value) {
        switch (static_cast<int>(value)) {
            case 0:
                return DirectionLeverPosition::Neutral;
            case 1:
                return DirectionLeverPosition::Forwards;
            case -1:
                return DirectionLeverPosition::Backwards;
            default:
                throw std::invalid_argument(std::to_string(value) + " does not correspond to a DirectionLeverPosition");
        }
    }
    
    inline double ToDouble(DirectionLeverPosition position) {
        return static_cast<int>(position);
    }
}
