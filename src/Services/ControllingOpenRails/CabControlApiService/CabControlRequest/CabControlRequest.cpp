#include "CabControlRequest.hpp"

std::string CabControlRequest::AsRequestBody() const {
    // Crafting the JSON manually, since Open Rails API does not recognize
    // a valid JSON object, but a JavaScript object.
    // See the function below this one for more info about this.
    
    std::ostringstream stream;
    stream << '[';
    bool first = true;
    for (const auto& item : this->items) {
        if (!first) {
            stream << ',';
        }
        stream << item;
        first = false;
    }
    stream << ']';
    
    return stream.str();
}

std::ostream& operator<<(std::ostream& stream, CabControlRequest::RequestItem item) {
    // Example of a correct representation: {TypeName: "THROTTLE", Value:0.4}
    // NOTE: This is not a valid JSON object. Correct one should be {"TypeName": "THROTTLE", "Value":0.4};
    //       This is the reason we have to create our own objects and don't use nlohmann::json.
    
    stream << "{TypeName: \"" << CabControlTypeToRequestString(item.controlType)
           << "\", Value:" << item.value << '}';
    return stream;
}

CabControlRequest& CabControlRequest::SetThrottle(double percentage) {
    this->items.emplace_back(CabControlType::Throttle, percentage);
    return *this;
}

CabControlRequest& CabControlRequest::SetTrainBrake(double percentage) {
    this->items.emplace_back(CabControlType::TrainBrake, percentage);
    return *this;
}

CabControlRequest& CabControlRequest::SetEngineBrake(double percentage) {
    this->items.emplace_back(CabControlType::EngineBrake, percentage);
    return *this;
}

CabControlRequest& CabControlRequest::SetDynamicBrake(double percentage) {
    this->items.emplace_back(CabControlType::DynamicBrake, percentage);
    return *this;
}

CabControlRequest& CabControlRequest::SetDirection(DirectionLeverPosition position) {
    this->items.emplace_back(
            CabControlType::Direction,
            DirectionLeverPositionMethods::ToDouble(position)
    );
    return *this;
}
