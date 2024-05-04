#include <stdexcept>
#include <sstream>
#include "CabControlApiService.hpp"

bool CabControlApiService::LpcSaidStart() {
    throw std::runtime_error("todo");
}

bool CabControlApiService::LpcSaidStop() {
    throw std::runtime_error("todo");
}

bool CabControlApiService::LpcSaidRestart() {
    throw std::runtime_error("todo");
}

void CabControlApiService::Clear() {
    this->itemsToSend.clear();
}

void CabControlApiService::SendAndClear() {
    throw std::runtime_error("todo");
    
    std::string requestBody = this->constructRequestBody();
    // TODO sending of request
    this->Clear();
}

CabControlApiService& CabControlApiService::SetThrottle(double percentage) {
    this->itemsToSend.emplace_back(OpenRailsControlElement::Throttle, percentage);
    return *this;
}

CabControlApiService& CabControlApiService::SetBrake(double percentage) {
    this->itemsToSend.emplace_back(OpenRailsControlElement::Brake, percentage);
    return *this;
}

CabControlApiService& CabControlApiService::SetDirection(DirectionLevelPosition position) {
    double value;
    switch (position) {
        case DirectionLevelPosition::Neutral:
            value = 0;
            break;
        case DirectionLevelPosition::Forwards:
            value = 1;
            break;
        case DirectionLevelPosition::Backwards:
            value = -1;
            break;
        default:
            throw std::runtime_error("unreachable branch");
    }
    
    this->itemsToSend.emplace_back(OpenRailsControlElement::Direction, value);
    return *this;
}

std::string CabControlApiService::constructRequestBody() {
    std::ostringstream stream("[");
    bool first = true;
    for (const auto& item : this->itemsToSend) {
        if (!first) {
            stream << ',';
        }
        item.PrintToStream(stream);
        first = false;
    }
    stream << ']';
    
    return stream.str();
}

void CabControlApiService::RequestItem::PrintToStream(std::ostream& stream) const {
    std::string controlTypeString;
    switch (this->controlType) {
        case OpenRailsControlElement::Throttle:
            stream << "THROTTLE";
            break;
        case OpenRailsControlElement::Brake:
            stream << "TRAIN_BRAKE";
            break;
        case OpenRailsControlElement::Direction:
            stream << "DIRECTION";
            break;
        default:
            throw std::runtime_error("unreachable branch");
    }
    
    stream << "{TypeName: \""
           << controlTypeString
           << "\", Value: "
           << this->value
           << "}";
}

CabControlApiService::RequestItem::RequestItem(OpenRailsControlElement controlType, double value)
        : controlType(controlType), value(value) {}