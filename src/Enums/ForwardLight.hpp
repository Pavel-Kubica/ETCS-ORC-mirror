/** @file ForwardLight.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of light positions
 *
 *  ### Contributors
 *  kubicpa3
 */

#pragma once

enum class ForwardLight {
    Off = 0,
    Day = 2,
    Far = 5
};

// TODO inconsistent behavior in OR
inline double toDouble(ForwardLight light) {
    return static_cast<double>(light);
}