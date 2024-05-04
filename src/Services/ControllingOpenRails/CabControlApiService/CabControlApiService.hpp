/** @file CabControlApi.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service for POSTing to the OpenRails API in order to manage
 *  the train's controls (brakes, throttle, ...).
 *
 *  ### Contributors
 *  rehorja8
 */

#pragma once


#include <vector>
#include <ostream>
#include "ICabControlApiService.hpp"
#include "OpenRailsControlElement.hpp"

class CabControlApiService : public ICabControlApiService,
                             public ILpcManageable {
public:
    
    void Clear() override;
    void SendAndClear() override;
    void SetThrottle(double percentage) override;
    void SetBrake(double percentage) override;
    void SetDirection(DirectionLevelPosition position) override;
    
    
    // LPC Management
    bool LpcSaidStart() override;
    bool LpcSaidStop() override;
    bool LpcSaidRestart() override;
    
private:
    struct RequestItem {
        OpenRailsControlElement controlType;
        double value;
        RequestItem(OpenRailsControlElement controlType, double value);
        void PrintToStream(std::ostream& stream) const;
    };
    
    std::string constructRequestBody();
    std::vector<RequestItem> itemsToSend;
};
