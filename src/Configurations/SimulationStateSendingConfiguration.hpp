/** @file SimulationStateSendingConfiguration.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Configuration of sending messages about simulation state
 *  See src/Services/Configuration/ConfigurationService for more info of how this works.
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once


#include "json.hpp"

class SimulationStateSendingConfiguration {
public:
    static constexpr char FileName[] = "simulation-state-sending-configuration.json";
    
    void from_json(const nlohmann::json& j);
    [[nodiscard]] nlohmann::json to_json() const;
    
    bool waitForOdoConfigMessage = false;
    std::chrono::milliseconds orcToGuiSendingInterval{500};
    std::chrono::milliseconds odoToEvcSendingInterval{500};     // this one is only used when waitForOdoConfigMessage is set to false
};
