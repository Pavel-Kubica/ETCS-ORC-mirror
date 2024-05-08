/** @file CabControlApi.cpp
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

#include <stdexcept>
#include <sstream>
#include "CabControlApiService.hpp"
#include "OpenRailsApiConfiguration.hpp"

bool CabControlApiService::LpcSaidStart() {
    this->LoadConfig();
    this->threadForResolvingResponses = std::thread(&CabControlApiService::ResolveResponses, this);
    return true;
}

void CabControlApiService::LoadConfig() {
    auto urlApiConfig = this->configurationService->FetchConfiguration<OpenRailsApiConfiguration>();
    this->url = urlApiConfig.cabControlsUrl;
    this->apiRequestTimeout = urlApiConfig.cabControlsTimeout;
}

bool CabControlApiService::LpcSaidStop() {
    this->responseQueue.Push(HttpResponseWrapper::CreateWrapperRepresentingFinalMessage());
    this->threadForResolvingResponses.join();
    return true;
}

bool CabControlApiService::LpcSaidRestart() {
    return this->LpcSaidStop() && this->LpcSaidStart();
}

void CabControlApiService::Initialize(ServiceContainer& container) {
    this->jruLoggerService = container.FetchService<JRULoggerService>().get();
    this->configurationService = container.FetchService<ConfigurationService>().get();
    
    // TODO: remove call to this method once the ORC is able to resolve messages from LPC.
    this->LoadConfig();
}

void CabControlApiService::Send(const CabControlRequest& request) {
    std::string requestBody = request.AsRequestBody();
    
    this->jruLoggerService->Log(true,
                                MessageType::Debug,
                                "CabControlApiService: doing post on OpenRails API with the request body: " +
                                requestBody);
    
    auto response = cpr::PostAsync(
            cpr::Url{this->url},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Timeout{this->apiRequestTimeout},
            cpr::Body{requestBody}
    );
    HttpResponseWrapper responseWrapper(
            std::move(response),
            std::move(requestBody)
    );
    
    this->responseQueue.Push(std::move(responseWrapper));
}

bool CabControlApiService::HttpResponseWrapper::RepresentsFinalMessage() const {
    return !(this->asyncResponse.has_value());
}

cpr::Response CabControlApiService::HttpResponseWrapper::GetResponse() {
    return this->asyncResponse.value().get();
}

CabControlApiService::HttpResponseWrapper
CabControlApiService::HttpResponseWrapper::CreateWrapperRepresentingFinalMessage() {
    return {};
}

CabControlApiService::HttpResponseWrapper::HttpResponseWrapper(
        cpr::AsyncWrapper<cpr::Response>&& response, std::string originalRequestBody)
        : asyncResponse(std::move(response)), originalRequestBody(std::move(originalRequestBody)) {}


void CabControlApiService::ResolveResponses() {
    while (true) {
        HttpResponseWrapper responseWrapper = this->responseQueue.Pop();
        if (responseWrapper.RepresentsFinalMessage()) {
            break;
        }
        
        cpr::Response response = responseWrapper.GetResponse();
        if (response.status_code / 100 != 2) {
            // Detected error
            std::ostringstream error;
            error << "CabControlApiService: OpenRails API returned non 2xx response code. "
                  << "[status code: " << response.status_code << ']'
                  << "[error: " << response.error.message << ']'
                  << "[original http request body: " << responseWrapper.originalRequestBody << ']';
            
            jruLoggerService->Log(true, MessageType::Error, error.str());
        } else {
            jruLoggerService->Log(
                    MessageType::Note,
                    "CabControlApiService: successful POST to OpenRails API. Original message: " +
                    responseWrapper.originalRequestBody
            );
        }
    }
}
