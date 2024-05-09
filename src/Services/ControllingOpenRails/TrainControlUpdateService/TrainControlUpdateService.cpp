/** @file HumanControlDataService.cpp
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
#include "TrainBrake.hpp"

void TrainControlUpdateService::Initialize(ServiceContainer& container) {
    cabControlApiService = container.FetchService<ICabControlApiService>().get();
    humanControlDataService = container.FetchService<IHumanControlDataService>().get();
    machineControlDataService = container.FetchService<IMachineControlDataService>().get();
    mqttPublisherService = container.FetchService<IMqttPublisherService>().get();
    incrementApiService = container.FetchService<IIncrementalCabControlService>().get();
    openRailsState = container.FetchService<ILocalCabControlsDataService>().get();
    configurationService = container.FetchService<ConfigurationService>().get();
}

bool TrainControlUpdateService::LpcSaidStart() {
    this->trainBrakeConfig = configurationService->FetchConfiguration<OpenRailsTrainBrakeConfiguration>();
    return true;
}

bool TrainControlUpdateService::LpcSaidStop() {
    return true;
}

bool TrainControlUpdateService::LpcSaidRestart() {
    return this->LpcSaidStop() && this->LpcSaidStart();
}

void TrainControlUpdateService::Update() {
    SendFromTiuMessageToEvc();
    SendOpenRailsCabControlsRequest();
}

void TrainControlUpdateService::SendFromTiuMessageToEvc() {
    Direction dir;
    switch (humanControlDataService->GetTrainDirection()) {
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
    FromTIUMessage message{humanControlDataService->GetBattery(), humanControlDataService->GetCab(),
                           dir};
    mqttPublisherService->Publish(std::make_shared<FromTIUMessage>(message));
}

void TrainControlUpdateService::SendOpenRailsCabControlsRequest() {
    if (!humanControlDataService->GetBattery() || !humanControlDataService->GetCab()) {
        return; // Cannot control the train if we are switched off
    }
    
    CabControlRequest apiRequest;
    apiRequest.SetDirection(humanControlDataService->GetTrainDirection());
    
    bool hadToHandleMachineInstructions = this->HandleMachineInstructions(apiRequest);
    if (!hadToHandleMachineInstructions) {
        this->HandleHumanInstructions(apiRequest);
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

bool TrainControlUpdateService::HandleMachineInstructions(CabControlRequest& request) {
    if (this->machineControlDataService->GetEmergencyBrake()) {
        this->SetThrottleInternal(0, request);
        this->SetEngineBrakeInternal(1, request);
        request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::EMERGENCY));
        return true;
    }
    
    if (this->machineControlDataService->GetServiceBrake()) {
        this->SetThrottleInternal(0, request);
        this->SetEngineBrakeInternal(1, request);
        request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL));
        return true;
    }
    
    return false;
}

void TrainControlUpdateService::HandleHumanInstructions(CabControlRequest& request) {
    switch (humanControlDataService->GetDrivingLever()) {
        case DrivingLeverPosition::Accelerate:
            this->incrementApiService->StartIncreasingThrottle();                                                        // THROTTLE
            this->SetEngineBrakeInternal(0, request);                                                            // ENGINE BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::QUICK_RELEASE)); // TRAIN BRAKE
            break;
        case DrivingLeverPosition::Continue:
            this->incrementApiService->StopChangingThrottle();                                                      // THROTTLE
            this->incrementApiService->StartDecreasingEngineBrake();                                               // ENGINE BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::RELEASE)); // TRAIN BRAKE
            break;
        case DrivingLeverPosition::Neutral:
            this->incrementApiService->StartDecreasingThrottle();                                                   // THROTTLE
            this->incrementApiService->StopChangingEngineBrake();                                                   // ENGINE BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL)); // TRAIN BRAKE
            break;
        case DrivingLeverPosition::ElectrodynamicBrake:
            this->SetThrottleInternal(0, request);                                                         // THROTTLE
            this->incrementApiService->StartIncreasingEngineBrake();                                               // ENGINE BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL)); // TRAIN BRAKE
            break;
        case DrivingLeverPosition::PneumaticBrake:
            this->SetThrottleInternal(0, request);                                                              // THROTTLE
            this->incrementApiService->StartIncreasingEngineBrake();                                                    // ENGINE BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::CONT_SERVICE)); // TRAIN BRAKE
            break;
        case DrivingLeverPosition::QuickBrake:
            this->SetThrottleInternal(0, request);                                                           // THROTTLE
            this->SetEngineBrakeInternal(1, request);                                                        // ENGINE BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::EMERGENCY)); // TRAIN BRAKE
            break;
    }
}

