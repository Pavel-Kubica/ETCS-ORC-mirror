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
#include "ForwardLight.hpp"

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
    CabControlRequest& SetPantograph(bool up);
    CabControlRequest& SetHorn(bool on);
    CabControlRequest& SetSander(bool on);
    CabControlRequest& SetEmergencyBrake(bool on);
    CabControlRequest& SetLight(ForwardLight light);

    CabControlRequest& Clear();
    
private:
    struct RequestItem {
        CabControlType controlType;
        double value;
    };
    friend std::ostream& operator<<(std::ostream& stream, RequestItem item);
    
    std::vector<RequestItem> items;
};
