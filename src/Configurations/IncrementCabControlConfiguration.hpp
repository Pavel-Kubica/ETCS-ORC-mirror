/** @file IncrementCabControlConfiguration.hpp
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

class IncrementCabControlConfiguration {
public:
    static constexpr char FileName[] = "increment-cab-controls-configuration.json";
    
    void from_json(const nlohmann::json& j);
    [[nodiscard]] nlohmann::json to_json() const;
    
     double throttleStep = 0.01;
     double brakeStep = 0.01;
     std::chrono::milliseconds timeBetweenIncrements{300};
};
