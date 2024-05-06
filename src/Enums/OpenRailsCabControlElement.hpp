/** @file OpenRailsCabControlElement.h
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of possible requests to OpenRails /API/CABCONTROLS/
 *
 *  ### Contributors
 *  rehorja8
 */

#pragma once

enum class OpenRailsCabControlElement {
    Throttle,
    Brake,
    Direction
};
