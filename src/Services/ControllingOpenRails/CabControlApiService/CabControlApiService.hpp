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
    // Implementation of ICabControlApiService required method.
    void Send(const CabControlRequest& request) override;
    
    // LPC Management
    bool LpcSaidStart() override;
    bool LpcSaidStop() override;
    bool LpcSaidRestart() override;
    
protected:
    void Initialize(ServiceContainer& container) override;

private:
    /**
     * Address of the Open Rails API. It is loaded from config.
     */
    std::string url;
    /**
     * Timeout that is set on every request to the Open Rails API.
     */
    std::chrono::milliseconds apiRequestTimeout;
    /**
     * Reloads `this->url` and `this->apiRequestTimeout` from config.
     */
    void LoadConfig();
    
    JRULoggerService* jruLoggerService;
    ConfigurationService* configurationService;
    
    /**
     * Wrapper around a single async response that is returned by POST to the Open Rails API.
     * This response is pushed into `this->responseQueue`.
     */
    struct HttpResponseWrapper {
        HttpResponseWrapper(cpr::AsyncWrapper<cpr::Response>&& response,
                            std::string originalRequestBody);
        /**
         * @return `HttpResponseWrapper` that represent that return true from RepresentsFinalMessage().
         * When this wrapper is popped from `responseQueue`, it should stop the worker thread that
         * resolves these responses.
         */
        static HttpResponseWrapper CreateWrapperRepresentingFinalMessage();
        
        std::optional<cpr::AsyncWrapper<cpr::Response>> asyncResponse;
        const std::string originalRequestBody;
        
        /**
         * @return True if after popping `this` wrapper from the `responseQueue`, the worker thread should stop.
         * False otherwise.
         */
        [[nodiscard]] bool RepresentsFinalMessage() const;
        
        /**
         * @throw std::bad_optional_access if `RepresentsFinalMessage` returns true.
         * @return the underlying response.
         */
        cpr::Response GetResponse();
    
    private:
        /**
         * Private default constructor.
         * The default object should be created by calling the `CreateWrapperRepresentingFinalMessage` function.
         */
        HttpResponseWrapper() = default;
    };
    
    AsyncQueue<HttpResponseWrapper> responseQueue;
    std::thread threadForResolvingResponses;
    
    /**
     * Called in the consumer thread. Logs errors from the HTTP responses.
     */
    void ResolveResponses();
};
