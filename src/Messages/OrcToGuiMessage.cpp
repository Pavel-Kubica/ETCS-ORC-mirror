
#include "OrcToGuiMessage.hpp"


OrcToGuiMessage::OrcToGuiMessage() {
    NID_MESSAGE = MessageID::OpenRailsSimulationStateORCtoGUI;
}

OrcToGuiMessage::OrcToGuiMessage(double speedMpS,
                                 double motiveForceN,
                                 double brakeCylinderPSI,
                                 double leadPipePSI) {
    NID_MESSAGE = MessageID::OpenRailsSimulationStateORCtoGUI;
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