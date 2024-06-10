/** @file ThrottleAndDynBrakeControlConfiguration.cpp
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

#include "ThrottleAndDynBrakeControlConfiguration.hpp"


constexpr char THROTTLE_STEP_STR[] = "throttle-step";
constexpr char BRAKE_STEP_STR[] = "brake-step";
constexpr char TIME_BETWEEN_INCREMENTS_STR[] = "time-between-increments-in-milliseconds";

void ThrottleAndDynBrakeControlConfiguration::from_json(const nlohmann::json& j) {
    this->throttleStep = j.at(THROTTLE_STEP_STR);
    this->brakeStep = j.at(BRAKE_STEP_STR);
    this->timeBetweenIncrements = std::chrono::milliseconds(j.at(TIME_BETWEEN_INCREMENTS_STR));
}

nlohmann::json ThrottleAndDynBrakeControlConfiguration::to_json() const {
    nlohmann::json j;
    
    j[THROTTLE_STEP_STR] = throttleStep;
    j[BRAKE_STEP_STR] = brakeStep;
    j[TIME_BETWEEN_INCREMENTS_STR] = timeBetweenIncrements.count();
    
    return j;
}
