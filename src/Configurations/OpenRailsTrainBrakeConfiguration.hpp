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
    
    /**
     * Converts train brake according to this loaded config to a value,
     * that can be POSTed to the Open Rails API so that the train brake in Open Rails' train
     * corresponds to `trainBrake`.
     * @param trainBrake - The train brake to be converted.
     * @return value of type double that can be POSTed to the Open Rails API.
    */
    [[nodiscard]] double ConvertToRequestValue(TrainBrake trainBrake) const;

private:
    double QuickRelease = 0;
    double Release = 0.2;
    double Neutral = 0.5;
    double Apply = 0.7;
    double Emergency = 4.2;
};
