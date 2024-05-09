/** @file OpenRailsTrainBrakeConfiguration.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Configuration of POST values of TrainBrake states.
 *  See `TrainBrake` enum for more info.
 *
 *  ### Contributors
 *  rehorja8
 */

#pragma once

#include "TrainBrake.hpp"
#include "json.hpp"

class OpenRailsTrainBrakeConfiguration {
public:
    static constexpr char FileName[] = "open-rails-train-brake-configuration.json";
    
    void from_json(const nlohmann::json& j);
    [[nodiscard]] nlohmann::json to_json() const;
    
    [[nodiscard]] double Convert(TrainBrake trainBrake) const;
private:
    int QuickRelease = 0;
    int Release = 30;       // stored as a percentage
    int Neutral = 60;       // stored as a percentage
    int ContService = 80;   // stored as a percentage
    int Emergency = 420;    // stored as a percentage
};
