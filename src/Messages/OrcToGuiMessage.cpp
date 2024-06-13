/** @file OrcToGuiMessage.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Orc data message to Gui applications
 *
 *  ###Contributors
 *  nguyem12
 */

#include "OrcToGuiMessage.hpp"


OrcToGuiMessage::OrcToGuiMessage() {
    NID_MESSAGE = MessageID::OpenRailsSimulationStateORCtoGUI;
    speedMpS = motiveForceN = brakeCylinderBar = leadPipeBar = mainReservoirPressureBar = 0;
}

OrcToGuiMessage::OrcToGuiMessage(double _speedMpS,
                                 double _motiveForceN,
                                 double _brakeCylinderBar,
                                 double _leadPipeBar,
                                 double _mainReservoirPressureBar) {
    NID_MESSAGE = MessageID::OpenRailsSimulationStateORCtoGUI;
    speedMpS = _speedMpS;
    motiveForceN = _motiveForceN;
    brakeCylinderBar = _brakeCylinderBar;
    leadPipeBar = _leadPipeBar;
    mainReservoirPressureBar = _mainReservoirPressureBar;
}

nlohmann::json OrcToGuiMessage::to_json() const {
    nlohmann::json j = Message::to_json();
    j["SPEED"] = speedMpS;
    j["MOTIVE_FORCE"] = motiveForceN;
    j["BRAKE_CYLINDER_BAR"] = brakeCylinderBar;
    j["LEAD_PIPE_BAR"] = leadPipeBar;
    j["MAIN_RESERVOIR_BAR"] = mainReservoirPressureBar;
    return j;
}

void OrcToGuiMessage::from_json(const nlohmann::json& j) {
    Message::from_json(j);
    speedMpS = j.at("SPEED").get<double>();
    motiveForceN = j.at("MOTIVE_FORCE").get<double>();
    brakeCylinderBar = j.at("BRAKE_CYLINDER_BAR").get<double>();
    leadPipeBar = j.at("LEAD_PIPE_BAR").get<double>();
    mainReservoirPressureBar = j.at("MAIN_RESERVOIR_BAR").get<double>();
}