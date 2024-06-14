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


constexpr char THROTTLE_INC_STEP_STR[] = "throttle-inc-step";
constexpr char ENGINE_BRAKE_INC_STEP_STR[] = "engine-brake-inc-step";
constexpr char DYNAMIC_BRAKE_INC_STEP_STR[] = "dynamic-brake-inc-step";
constexpr char THROTTLE_DEC_STEP_STR[] = "throttle-dec-step";
constexpr char ENGINE_BRAKE_DEC_STEP_STR[] = "engine-brake-dec-step";
constexpr char DYNAMIC_BRAKE_DEC_STEP_STR[] = "dynamic-brake-dec-step";
constexpr char TIME_BETWEEN_INCREMENTS_STR[] = "time-between-increments-in-milliseconds";

void IncrementalCabControlConfiguration::from_json(const nlohmann::json& j) {
    this->throttleIncStep = j.at(THROTTLE_INC_STEP_STR);
    this->dynamicBrakeIncStep = j.at(DYNAMIC_BRAKE_INC_STEP_STR);
    this->engineBrakeIncStep = j.at(ENGINE_BRAKE_INC_STEP_STR);
    this->throttleDecStep = j.at(THROTTLE_DEC_STEP_STR);
    this->dynamicBrakeDecStep = j.at(DYNAMIC_BRAKE_DEC_STEP_STR);
    this->engineBrakeDecStep = j.at(ENGINE_BRAKE_DEC_STEP_STR);

    this->timeBetweenIncrements = std::chrono::milliseconds(j.at(TIME_BETWEEN_INCREMENTS_STR));
}

nlohmann::json IncrementalCabControlConfiguration::to_json() const {
    nlohmann::json j;
    
    j[THROTTLE_INC_STEP_STR] = throttleIncStep;
    j[DYNAMIC_BRAKE_INC_STEP_STR] = dynamicBrakeIncStep;
    j[ENGINE_BRAKE_INC_STEP_STR] = engineBrakeIncStep;
    j[THROTTLE_DEC_STEP_STR] = throttleDecStep;
    j[DYNAMIC_BRAKE_DEC_STEP_STR] = dynamicBrakeDecStep;
    j[ENGINE_BRAKE_DEC_STEP_STR] = engineBrakeDecStep;

    j[TIME_BETWEEN_INCREMENTS_STR] = timeBetweenIncrements.count();
    
    return j;
}
