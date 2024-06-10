/** @file CabControlRequest.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Class representing a single request to the OpenRails API.
 *
 *  ### Contributors
 *  rehorja8
 */
#pragma once


#include <ostream>
#include "DirectionLeverPosition.hpp"
#include "CabControlType.hpp"
#include "json.hpp"

class CabControlRequest {
public:
    /**
     * @return This CabControlRequest as a body of HTTP request to the Open Rails API.
     */
    [[nodiscard]] std::string AsRequestBody() const;
    
    CabControlRequest& SetThrottle(double percentage);
    CabControlRequest& SetTrainBrake(double percentage);
    CabControlRequest& SetEngineBrake(double percentage);
    CabControlRequest& SetDynamicBrake(double percentage);
    CabControlRequest& SetDirection(DirectionLeverPosition position);

    CabControlRequest& Clear();
    
private:
    struct RequestItem {
        CabControlType controlType;
        double value;
    };
    friend std::ostream& operator<<(std::ostream& stream, RequestItem item);
    
    std::vector<RequestItem> items;
};
