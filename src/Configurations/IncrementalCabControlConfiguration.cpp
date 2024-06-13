/** @file IncrementalCabControlConfiguration.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Configuration of OpenRails' URL API endpoint.
 *  See src/Services/Configuration/ConfigurationService for more info of how this works.
 *
 *  ### Contributors
 *  rehorja8
 */

#include "IncrementalCabControlConfiguration.hpp"


constexpr char THROTTLE_STEP_STR[] = "throttle-step";
constexpr char ENGINE_BRAKE_STEP_STR[] = "engine-brake-step";
constexpr char DYNAMIC_BRAKE_STEP_STR[] = "dynamic-brake-step";
constexpr char TIME_BETWEEN_INCREMENTS_STR[] = "time-between-increments-in-milliseconds";

void IncrementalCabControlConfiguration::from_json(const nlohmann::json& j) {
    this->throttleStep = j.at(THROTTLE_STEP_STR);
    this->dynamicBrakeStep = j.at(DYNAMIC_BRAKE_STEP_STR);
    this->engineBrakeStep = j.at(ENGINE_BRAKE_STEP_STR);
    this->timeBetweenIncrements = std::chrono::milliseconds(j.at(TIME_BETWEEN_INCREMENTS_STR));
}

nlohmann::json IncrementalCabControlConfiguration::to_json() const {
    nlohmann::json j;
    
    j[THROTTLE_STEP_STR] = throttleStep;
    j[DYNAMIC_BRAKE_STEP_STR] = dynamicBrakeStep;
    j[ENGINE_BRAKE_STEP_STR] = engineBrakeStep;
    j[TIME_BETWEEN_INCREMENTS_STR] = timeBetweenIncrements.count();
    
    return j;
}
