/** @file CabControlApi.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Implementation of the ICabControlApiService interface (more info there).
 *
 *  ### Contributors
 *  rehorja8
 */

#pragma once


#include <cpr/cpr.h>
#include <vector>
#include <ostream>
#include "ICabControlApiService.hpp"
#include "OpenRailsCabControlElement.hpp"
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
    void SetTrainBrake(double percentage) override;
    void SetEngineBrake(double percentage) override;
    void SetDynamicBrake(double percentage) override;
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
    
    /**
     * Struct for storing individual cab controls and their state, so they
     * can be send in a bulk to the OpenRails API.
     * These are stored in the `itemsToSend` vector.
     */
    struct RequestItem {
        OpenRailsCabControlElement controlType;
        double value;
        RequestItem(OpenRailsCabControlElement controlType, double value);
        void PrintToStream(std::ostream& stream) const;
    };
    
    /**
     * Struct for storing async responses to OpenRails API calls.
     * These are stored in the `responseQueue`.
     * There can also be an "empty" instance, signalling to the consumer thread (made from the ResolveResponses() method)
     * that it should stop.
     */
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
    
    /**
     * Called in the consumer thread. Logs error from the HTTP responses.
     */
    void ResolveResponses();
    
    /**
     * Creates the body of an HTTP request based on `this->itemsToSend` vector.
     * @return The crated body as a string.
     */
    std::string ConstructRequestBody();
};
