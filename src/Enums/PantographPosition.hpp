/** @file PantographPosition.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enumerates possible states of the pantograph control on the control board.
 *
 *  ### Contributors
 *  cajantom
 */

#pragma once

enum class PantographPosition {
    Down,
    Up,
    UpConnected
};
