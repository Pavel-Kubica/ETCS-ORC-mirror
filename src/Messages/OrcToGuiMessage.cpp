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
    speedMpS = motiveForceN = brakeCylinderPSI = leadPipePSI = 0;
}

OrcToGuiMessage::OrcToGuiMessage(double _speedMpS,
                                 double _motiveForceN,
                                 double _brakeCylinderPSI,
                                 double _leadPipePSI) {
    NID_MESSAGE = MessageID::OpenRailsSimulationStateORCtoGUI;
    speedMpS = _speedMpS;
    motiveForceN = _motiveForceN;
    brakeCylinderPSI = _brakeCylinderPSI;
    leadPipePSI = _leadPipePSI;
}

nlohmann::json OrcToGuiMessage::to_json() const {
    nlohmann::json j = Message::to_json();
    j["SPEED"] = speedMpS;
    j["MOTIVE_FORCE"] = motiveForceN;
    j["BRAKE_CYLINDER"] = brakeCylinderPSI;
    j["LEAD_PIPE"] = leadPipePSI;
    return j;
}

void OrcToGuiMessage::from_json(const nlohmann::json& j) {
    Message::from_json(j);
    speedMpS = j.at("SPEED").get<double>();
    motiveForceN = j.at("MOTIVE_FORCE").get<double>();
    brakeCylinderPSI = j.at("BRAKE_CYLINDER").get<double>();
    leadPipePSI = j.at("LEAD_PIPE").get<double>();
}