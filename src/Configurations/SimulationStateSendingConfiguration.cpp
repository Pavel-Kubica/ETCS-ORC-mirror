/** @file SimulationStateSendingConfiguration.cpp
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
 
#include "SimulationStateSendingConfiguration.hpp"

constexpr char WAIT_FOR_ODO_CONFIG_MESSAGE[] = "wait-for-odo-config-message";
constexpr char ORC_TO_GUI_SENDING_INTERVAL[] = "orc-to-gui-sending-interval";
constexpr char ODO_TO_EVC_SENDING_INTERVAL[] = "odo-to-evc-sending-interval";


void SimulationStateSendingConfiguration::from_json(const nlohmann::json& j) {
    waitForOdoConfigMessage = j.at(WAIT_FOR_ODO_CONFIG_MESSAGE);
    orcToGuiSendingInterval = std::chrono::milliseconds{j.at(ORC_TO_GUI_SENDING_INTERVAL)};
    odoToEvcSendingInterval = std::chrono::milliseconds{j.at(ODO_TO_EVC_SENDING_INTERVAL)};
}

nlohmann::json SimulationStateSendingConfiguration::to_json() const {
    nlohmann::json j;

    j[WAIT_FOR_ODO_CONFIG_MESSAGE] = waitForOdoConfigMessage;
    j[ORC_TO_GUI_SENDING_INTERVAL] = orcToGuiSendingInterval.count();
    j[ODO_TO_EVC_SENDING_INTERVAL] = odoToEvcSendingInterval.count();
    
    return j;
}
