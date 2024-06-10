#include "OpenRailsTrainBrakeConfiguration.hpp"

#include <stdexcept>

constexpr char QUICK_RELEASE_STR[] = "QUICK_RELEASE";
constexpr char RELEASE_STR[] = "RELEASE";
constexpr char NEUTRAL_STR[] = "NEUTRAL";
constexpr char CONT_SERVICE_STR[] = "CONT_SERVICE";
constexpr char EMERGENCY_STR[] = "EMERGENCY";

double OpenRailsTrainBrakeConfiguration::ConvertToRequestValue(TrainBrake trainBrake) const {
    switch (trainBrake) {
        case TrainBrake::QUICK_RELEASE:
            return this->QuickRelease;
        case TrainBrake::RELEASE:
            return this->Release;
        case TrainBrake::NEUTRAL:
            return this->Neutral;
        case TrainBrake::APPLY:
            return this->Apply;
        case TrainBrake::EMERGENCY:
            return this->Emergency;
        default:
            throw std::invalid_argument("could not convert this case of TrainBrake enum");
    }
}

void OpenRailsTrainBrakeConfiguration::from_json(const nlohmann::json& j) {
    QuickRelease = j.at(QUICK_RELEASE_STR);
    Release = j.at(RELEASE_STR);
    Neutral = j.at(NEUTRAL_STR);
    Apply = j.at(CONT_SERVICE_STR);
    Emergency = j.at(EMERGENCY_STR);
}


nlohmann::json OpenRailsTrainBrakeConfiguration::to_json() const {
    nlohmann::json j;
    
    j[QUICK_RELEASE_STR] = QuickRelease;
    j[RELEASE_STR] = Release;
    j[NEUTRAL_STR] = Neutral;
    j[CONT_SERVICE_STR] = Apply;
    j[EMERGENCY_STR] = Emergency;
    
    return j;
}
