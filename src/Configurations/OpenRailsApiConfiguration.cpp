/** @file OpenRailsApiConfiguration.cpp
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
 
#include "OpenRailsApiConfiguration.hpp"

constexpr char CABCONTROLS[] = "cabcontrols";
constexpr char ORC[] = "orc";

constexpr char URL[] = "url";
constexpr char TIMEOUT[] = "timeout-in-milliseconds";

constexpr char API_GET_INTERVAL[] = "api-get-calling-interval-in-milliseconds";

void OpenRailsApiConfiguration::from_json(const nlohmann::json& j) {
    this->cabControlsUrl = j.at(CABCONTROLS).at(URL);
    this->orcUrl = j.at(ORC).at(URL);
    
    this->cabControlsTimeout = std::chrono::milliseconds{j.at(CABCONTROLS).at(TIMEOUT)};
    this->orcTimeout = std::chrono::milliseconds{j.at(ORC).at(TIMEOUT)};
    
    this->apiGetOrcCallingInterval = std::chrono::milliseconds{j.at(ORC).at(API_GET_INTERVAL)};
}

nlohmann::json OpenRailsApiConfiguration::to_json() const {
    nlohmann::json j;
    
    j[CABCONTROLS][URL] = this->cabControlsUrl;
    j[CABCONTROLS][TIMEOUT] = this->cabControlsTimeout.count();
    
    j[ORC][URL] = this->orcUrl;
    j[ORC][TIMEOUT] = this->orcTimeout.count();
    
    j[ORC][API_GET_INTERVAL] = this->apiGetOrcCallingInterval.count();
    
    return j;
}
