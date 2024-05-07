#include "CabControlRequest.hpp"

std::string CabControlRequest::AsRequestBody() const {
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

std::ostream& operator<<(std::ostream& stream, CabControlRequest::RequestItem item) {
    // Example: {TypeName: "THROTTLE", Value:0.4}
    stream << "{TypeName: \"" << CabControlTypeToRequestString(item.controlType)
           << "\", Value:" << item.value << '}';
    return stream;
}
// [{TypeName: "DIRECTION", Value: 1}]