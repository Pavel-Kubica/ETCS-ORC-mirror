#pragma once


#include <ostream>
#include "DirectionLeverPosition.hpp"
#include "CabControlType.hpp"
#include "json.hpp"

class CabControlRequest {
public:
    [[nodiscard]] std::string AsRequestBody() const;
    
    CabControlRequest& SetThrottle(double percentage);
    CabControlRequest& SetTrainBrake(double percentage);
    CabControlRequest& SetEngineBrake(double percentage);
    CabControlRequest& SetDynamicBrake(double percentage);
    CabControlRequest& SetDirection(DirectionLeverPosition position);
private:
    struct RequestItem {
        CabControlType controlType;
        double value;
        
    };
    friend std::ostream& operator<<(std::ostream& stream, RequestItem item);
    
    std::vector<RequestItem> items;
};
