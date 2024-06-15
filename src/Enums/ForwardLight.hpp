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
    Far = 4
};

// Orts.[RunActivity].Viewer3D.RollingStock.MSTSLocomotiveViewer:277
inline double toDouble(ForwardLight light) {
    return static_cast<double>(light);
}