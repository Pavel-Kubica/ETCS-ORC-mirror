/** @file ThrottleAndDynBrakeControlConfiguration.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Configuration for ThrottleAndDynBrakeControlsService.
 *
 *  ### Contributors
 *  rehorja8
 */
 
#pragma once

#include <chrono>
#include "json.hpp"

class ThrottleAndDynBrakeControlConfiguration {
public:
    static constexpr char FileName[] = "incremental-cab-controls-configuration.json";
    
    void from_json(const nlohmann::json& j);
    [[nodiscard]] nlohmann::json to_json() const;
    
     double throttleStep = 0.01;
     double dynamicBrakeStep = 0.01;
     double engineBrakeStep = 0.01;
     std::chrono::milliseconds timeBetweenIncrements{300};
};
