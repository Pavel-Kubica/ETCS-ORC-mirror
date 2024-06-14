/** @file IncrementalCabControlConfiguration.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Configuration for IncrementalCabControlsService.
 *
 *  ### Contributors
 *  rehorja8
 */
 
#pragma once

#include <chrono>
#include "json.hpp"

class IncrementalCabControlConfiguration {
public:
    static constexpr char FileName[] = "incremental-cab-controls-configuration.json";
    
    void from_json(const nlohmann::json& j);
    [[nodiscard]] nlohmann::json to_json() const;
    
     double throttleIncStep = 0.01;
     double dynamicBrakeIncStep = 0.03;
     double engineBrakeIncStep = 0.03;
     double throttleDecStep = 0.03;
     double dynamicBrakeDecStep = 0.01;
     double engineBrakeDecStep = 0.01;
     std::chrono::milliseconds timeBetweenIncrements{300};
};
