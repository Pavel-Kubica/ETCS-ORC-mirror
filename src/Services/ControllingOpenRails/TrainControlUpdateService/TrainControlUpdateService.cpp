/** @file TrainControlDataService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  This service reads the current status of train controls and sends it to EVC and Open Rails cab control
 *
 *  ###Contributors
 *  kubicpa3
 *  rehorja8
 */

#include "TrainControlUpdateService.hpp"
#include "ServiceContainer.hpp"

void TrainControlUpdateService::Initialize(ServiceContainer& container) {
    cabControlApiService = container.FetchService<ICabControlApiService>().get();
    trainControlDataService = container.FetchService<ITrainControlDataService>().get();
    machineControlDataService = container.FetchService<IMachineControlDataService>().get();
    mqttPublisherService = container.FetchService<IMqttPublisherService>().get();
    incrementApiService = container.FetchService<IIncrementalCabControlService>().get();
    openRailsState = container.FetchService<ILocalCabControlsDataService>().get();
}

void TrainControlUpdateService::Update() {
    SendFromTiuMessageToEvc();
    SendOpenRailsCabControlsRequest();
}

void TrainControlUpdateService::SendFromTiuMessageToEvc() {
    Direction dir;
    switch (trainControlDataService->GetTrainDirection()) {
        case DirectionLeverPosition::Neutral:
            dir = Direction::Unknown;
            break;
        case DirectionLeverPosition::Forwards:
            dir = Direction::Nominal;
            break;
        case DirectionLeverPosition::Backwards:
            dir = Direction::Reverse;
            break;
        default:
            throw std::runtime_error("unreachable branch");
    }
    FromTIUMessage message{trainControlDataService->GetBattery(), trainControlDataService->GetCab(),
                           dir};
    mqttPublisherService->Publish(std::make_shared<FromTIUMessage>(message));
}

void TrainControlUpdateService::SendOpenRailsCabControlsRequest() {
    if (!trainControlDataService->GetBattery() || !trainControlDataService->GetCab()) {
        return; // Cannot control the train if we are switched off
    }
    
    CabControlRequest apiRequest;
    apiRequest.SetDirection(trainControlDataService->GetTrainDirection());
    
    // TODO: brake, train brake
    
    switch (trainControlDataService->GetDrivingLever()) {
        case DrivingLeverPosition::Accelerate:
            this->incrementApiService->StartIncreasingThrottle();
            this->SetEngineBrakeInternal(0, apiRequest);
            break;
        case DrivingLeverPosition::Continue:
            this->incrementApiService->StopChangingThrottle();
            this->incrementApiService->StartDecreasingEngineBrake();
            break;
        case DrivingLeverPosition::Neutral:
            this->incrementApiService->StartDecreasingThrottle();
            this->incrementApiService->StopChangingEngineBrake();
            break;
        case DrivingLeverPosition::ElectrodynamicBrake:
            this->SetThrottleInternal(0, apiRequest);
            this->incrementApiService->StartIncreasingEngineBrake();
            break;
        case DrivingLeverPosition::PneumaticBrake:
            this->SetThrottleInternal(0, apiRequest);
            this->incrementApiService->StartIncreasingEngineBrake();
            break;
        case DrivingLeverPosition::QuickBrake:
            this->SetThrottleInternal(0, apiRequest);
            this->SetEngineBrakeInternal(1, apiRequest);
            break;
    }
    
    this->cabControlApiService->Send(apiRequest);
}

void TrainControlUpdateService::SetThrottleInternal(double value, CabControlRequest& request) {
    this->incrementApiService->StopChangingThrottle();
    request.SetThrottle(value);
    this->openRailsState->SetThrottle(value);
}

void TrainControlUpdateService::SetEngineBrakeInternal(double value, CabControlRequest& request) {
    this->incrementApiService->StopChangingEngineBrake();
    request.SetEngineBrake(value);
    this->openRailsState->SetEngineBrake(value);
}

