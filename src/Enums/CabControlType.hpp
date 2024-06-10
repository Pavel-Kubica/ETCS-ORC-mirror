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

enum class CabControlType {
    Throttle,
    TrainBrake,
    EngineBrake,
    DynamicBrake,
    Direction,
    Pantograph,
    Horn,
    Sander,
    EmergencyBrake
};

inline std::string_view CabControlTypeToRequestString(CabControlType type) {
    switch (type) {
        case CabControlType::Throttle:
            return "THROTTLE";
        case CabControlType::TrainBrake:
            return "TRAIN_BRAKE";
        case CabControlType::EngineBrake:
            return "ENGINE_BRAKE";
        case CabControlType::DynamicBrake:
            return "DYNAMIC_BRAKE";
        case CabControlType::Direction:
            return "DIRECTION";
        case CabControlType::Pantograph:
            return "PANTOGRAPH";
        case CabControlType::Horn:
            return "HORN";
        case CabControlType::Sander:
            return "SANDERS";
        case CabControlType::EmergencyBrake:
            return "EMERGENCY_BRAKE";
        default:
            throw std::runtime_error("unreachable branch");
    }
}