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
#include "CabControlType.hpp"
#include "AsyncQueue.hpp"
#include "IInitializable.hpp"
#include "ILpcManageable.hpp"
#include "JRULoggerService.hpp"
#include "ConfigurationService.hpp"

class CabControlApiService : public ICabControlApiService,
                             public IInitializable,
                             public ILpcManageable {
public:
protected:
    void Initialize(ServiceContainer& container) override;
public:
    
    void Send(const CabControlRequest& request) override;
    
    // LPC Management
    bool LpcSaidStart() override;
    bool LpcSaidStop() override;
    bool LpcSaidRestart() override;

private:
    std::string url;
    std::chrono::milliseconds apiRequestTimeout;
    
    JRULoggerService* jruLoggerService;
    ConfigurationService* configurationService;
    
    struct HttpResponseWrapper {
        HttpResponseWrapper(cpr::AsyncWrapper<cpr::Response>&& response,
                            std::string originalRequestBody);
        
        static HttpResponseWrapper CreateWrapperRepresentingFinalMessage();
        std::optional<cpr::AsyncWrapper<cpr::Response>> asyncResponse;
        const std::string originalRequestBody;
        
        [[nodiscard]] bool RepresentsFinalMessage() const;
        cpr::Response GetResponse();
    
    private:
        HttpResponseWrapper() = default;
    };
    
    AsyncQueue<HttpResponseWrapper> responseQueue;
    std::thread threadForResolvingResponses;
    
    /**
     * Called in the consumer thread. Logs errors from the HTTP responses.
     */
    void ResolveResponses();
    
    void LoadConfig();
};
