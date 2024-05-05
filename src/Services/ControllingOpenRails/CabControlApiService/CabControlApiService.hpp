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


#include <cpr/cpr.h>
#include <vector>
#include <ostream>
#include "ICabControlApiService.hpp"
#include "OpenRailsControlElement.hpp"
#include "AsyncQueue.hpp"
#include "IInitializable.hpp"
#include "ILpcManageable.hpp"
#include "JRULoggerService.hpp"

class CabControlApiService : public ICabControlApiService,
                             public IInitializable,
                             public ILpcManageable {
public:
protected:
    void Initialize(ServiceContainer& container) override;
public:
    void Clear() override;
    void SendAndClear() override;
    
    void SetThrottle(double percentage) override;
    void SetBrake(double percentage) override;
    void SetDirection(DirectionLeverPosition position) override;
    
    // LPC Management
    bool LpcSaidStart() override;
    bool LpcSaidStop() override;
    bool LpcSaidRestart() override;

private:
    
    // TODO: put this in a config
    const std::string url = "http://127.0.0.1:2150/API/";
    const std::string end_point = "CABCONTROLS";
    const int32_t requestTimeoutInMilliseconds = 1000;
    
    JRULoggerService* jruLoggerService;
    
    struct RequestItem {
        OpenRailsControlElement controlType;
        double value;
        RequestItem(OpenRailsControlElement controlType, double value);
        void PrintToStream(std::ostream& stream) const;
    };
    
    struct HttpResponseWrapper {
        explicit HttpResponseWrapper(
                cpr::AsyncWrapper<cpr::Response>&& response,
                std::string originalRequestBody,
                std::vector<RequestItem> originalRequestCabControls
        );
        
        static HttpResponseWrapper CreateWrapperRepresentingFinalMessage();
        
        std::optional<cpr::AsyncWrapper<cpr::Response>> asyncResponse;
        const std::string originalRequestBody;
        const std::vector<RequestItem> originalRequestCabControls;
        
        [[nodiscard]] bool RepresentsFinalMessage() const;
        cpr::Response GetResponse();
    
    private:
        HttpResponseWrapper() = default;
    };
    
    std::vector<RequestItem> itemsToSend;
    AsyncQueue<HttpResponseWrapper> responseQueue;
    std::thread threadForResolvingResponses;
    
    void ResolveResponses();
    std::string ConstructRequestBody();
};
