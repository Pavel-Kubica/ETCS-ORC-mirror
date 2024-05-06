#include <stdexcept>
#include <sstream>
#include "CabControlApiService.hpp"

bool CabControlApiService::LpcSaidStart() {
    this->threadForResolvingResponses = std::thread(&CabControlApiService::ResolveResponses, this);
    return true;
}

bool CabControlApiService::LpcSaidStop() {
    this->responseQueue.Push(HttpResponseWrapper::CreateWrapperRepresentingFinalMessage());
    this->threadForResolvingResponses.join();
    return true;
}

bool CabControlApiService::LpcSaidRestart() {
    return this->LpcSaidStop() && this->LpcSaidStart();
}

void CabControlApiService::Clear() {
    this->itemsToSend.clear();
}

void CabControlApiService::SendAndClear() {
    std::string requestBody = this->ConstructRequestBody();
    
    this->jruLoggerService->Log(MessageType::Debug,
                                "CabControlApiService: doing post on OpenRails API with the request body: " +
                                requestBody);
    
    auto response = cpr::PostAsync(
            cpr::Url{this->url + this->end_point},
            cpr::Header{{"Content-Type", "application/json"}},
            cpr::Timeout{this->requestTimeoutInMilliseconds},
            cpr::Body{std::string_view(requestBody)}
    );
    HttpResponseWrapper responseWrapper(
            std::move(response),
            std::move(requestBody),
            std::move(this->itemsToSend)
    );
    
    this->responseQueue.Push(std::move(responseWrapper));
    this->Clear();
}

void CabControlApiService::SetThrottle(double percentage) {
    this->itemsToSend.emplace_back(OpenRailsCabControlElement::Throttle, percentage);
}

void CabControlApiService::SetTrainBrake(double percentage) {
    this->itemsToSend.emplace_back(OpenRailsCabControlElement::TrainBrake, percentage);
}

void CabControlApiService::SetEngineBrake(double percentage) {
    this->itemsToSend.emplace_back(OpenRailsCabControlElement::EngineBrake, percentage);
}

void CabControlApiService::SetDynamicBrake(double percentage) {
    this->itemsToSend.emplace_back(OpenRailsCabControlElement::DynamicBrake, percentage);
}

void CabControlApiService::SetDirection(DirectionLeverPosition position) {
    double value = DirectionLeverPositionMethods::ToDouble(position);
    this->itemsToSend.emplace_back(OpenRailsCabControlElement::Direction, value);
}

std::string CabControlApiService::ConstructRequestBody() {
    std::ostringstream stream;
    stream << '[';
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
                    MessageType::Debug,
                    "CabControlApiService: successful POST to OpenRails API. Original message: " +
                    responseWrapper.originalRequestBody
            );
        }
    }
}

void CabControlApiService::Initialize(ServiceContainer& container) {
    this->jruLoggerService = container.FetchService<JRULoggerService>().get();
}

void CabControlApiService::RequestItem::PrintToStream(std::ostream& stream) const {
    stream << "{TypeName: \"";
    
    switch (this->controlType) {
        case OpenRailsCabControlElement::Throttle:
            stream << "THROTTLE";
            break;
        case OpenRailsCabControlElement::TrainBrake:
            stream << "TRAIN_BRAKE";
            break;
        case OpenRailsCabControlElement::EngineBrake:
            stream << "ENGINE_BRAKE";
            break;
        case OpenRailsCabControlElement::DynamicBrake:
            stream << "DYNAMIC_BRAKE";
            break;
        case OpenRailsCabControlElement::Direction:
            stream << "DIRECTION";
            break;
        default:
            throw std::runtime_error("unreachable branch");
    }
    
    stream << "\", Value: "
           << this->value
           << "}";
}

CabControlApiService::RequestItem::RequestItem(OpenRailsCabControlElement
                                               controlType, double
                                               value)
        : controlType(controlType), value(value) {}

bool CabControlApiService::HttpResponseWrapper::RepresentsFinalMessage() const {
    return !(this->asyncResponse.has_value());
}

cpr::Response CabControlApiService::HttpResponseWrapper::GetResponse() {
    return this->asyncResponse->get();
}

CabControlApiService::HttpResponseWrapper::HttpResponseWrapper(
        cpr::AsyncWrapper<cpr::Response>&& response,
        std::string originalRequestBody,
        std::vector<RequestItem> originalRequestCabControls)
        : asyncResponse(std::move(response)),
          originalRequestBody(std::move(originalRequestBody)),
          originalRequestCabControls(std::move(originalRequestCabControls)) {}

CabControlApiService::HttpResponseWrapper
CabControlApiService::HttpResponseWrapper::CreateWrapperRepresentingFinalMessage() {
    return {};
}


