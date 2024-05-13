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
    jruLoggerService = container.FetchService<JRULoggerService>().get();

    // This causes to create a config file if one does not exist.
    // This is handy so that we can just run the program once without LpcSaidStart
    // and the configuration gets created.
    configurationService->FetchConfiguration<OpenRailsTrainBrakeConfiguration>();
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
    FromTIUMessage message{humanControlDataService->GetBattery(), humanControlDataService->GetCab(), dir};
    mqttPublisherService->Publish(std::make_shared<FromTIUMessage>(message));
}

void TrainControlUpdateService::SendOpenRailsCabControlsRequest() {
    bool battery = humanControlDataService->GetBattery();
    bool cab = humanControlDataService->GetCab();
    if (!battery || !cab) {
        jruLoggerService->Log(
            true, MessageType::Note,
            "TrainControlUpdateService: (small) warning - no action was sent to train because the cab or "
            "battery is off: [battery:%battery%] [cab:%cab%]",
            battery, cab);
        return;  // Cannot control the train if we are switched off
    }

    CabControlRequest apiRequest;
    apiRequest.SetDirection(humanControlDataService->GetTrainDirection());

    bool hadToHandleMachineInstructions = this->HandleMachineInstructions(apiRequest);
    if (!hadToHandleMachineInstructions) {
        this->HandleHumanInstructions(apiRequest);
    }

    this->cabControlApiService->Send(apiRequest);
}

bool TrainControlUpdateService::HandleMachineInstructions(CabControlRequest& request) {
    if (this->machineControlDataService->GetEmergencyBrake()) {
        this->incrementApiService->SetThrottleTo(0);
        request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::EMERGENCY));
        this->openRailsState->SetTrainBrake(TrainBrake::EMERGENCY);
        return true;
    }

    if (this->machineControlDataService->GetServiceBrake()) {
        this->incrementApiService->SetThrottleTo(0);
        request.SetEngineBrake(1);
        request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL));
        return true;
    }
    
    return false;
}

void TrainControlUpdateService::HandleHumanInstructions(CabControlRequest& request) {
    switch (humanControlDataService->GetDrivingLever()) {
        case DrivingLeverPosition::Accelerate:
            this->incrementApiService->StartIncreasingThrottle();                                // THROTTLE
            this->incrementApiService->SetDynamicBrakeTo(0);                                     // DYNAMIC BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::RELEASE));  // TRAIN BRAKE
            openRailsState->SetTrainBrake(TrainBrake::RELEASE);
            break;
        case DrivingLeverPosition::Continue:
            this->incrementApiService->StopChangingThrottle();                                   // THROTTLE
            this->incrementApiService->StartDecreasingDynamicBrake();                            // DYNAMIC BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::RELEASE));  // TRAIN BRAKE
            openRailsState->SetTrainBrake(TrainBrake::RELEASE);
            break;
        case DrivingLeverPosition::Neutral:
            this->incrementApiService->StartDecreasingThrottle();   // THROTTLE
            this->incrementApiService->StopChangingDynamicBrake();  // DYNAMIC BRAKE
            if (humanControlDataService->HasTouchedRelease()) {     // TRAIN BRAKE
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::RELEASE));
                openRailsState->SetTrainBrake(TrainBrake::RELEASE);
            } else {
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL));
                openRailsState->SetTrainBrake(TrainBrake::NEUTRAL);
            }
            break;
        case DrivingLeverPosition::ElectrodynamicBrake:
            this->incrementApiService->SetThrottleTo(0);               // THROTTLE
            this->incrementApiService->StartIncreasingDynamicBrake();  // DYNAMIC BRAKE
            if (humanControlDataService->HasTouchedRelease()) {        // TRAIN BRAKE
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::RELEASE));
                openRailsState->SetTrainBrake(TrainBrake::RELEASE);
            } else {
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL));
                openRailsState->SetTrainBrake(TrainBrake::NEUTRAL);
            }
            break;
        case DrivingLeverPosition::PneumaticBrake:
            this->incrementApiService->SetThrottleTo(0);                                       // THROTTLE
            this->incrementApiService->StopChangingDynamicBrake();                             // DYNAMIC BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::APPLY));  // TRAIN BRAKE
            openRailsState->SetTrainBrake(TrainBrake::APPLY);
            break;
        case DrivingLeverPosition::QuickBrake:
            this->incrementApiService->SetThrottleTo(0);                                           // THROTTLE
            this->incrementApiService->SetDynamicBrakeTo(0);                                       // DYNAMIC BRAKE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::EMERGENCY));  // TRAIN BRAKE
            openRailsState->SetTrainBrake(TrainBrake::EMERGENCY);
            break;
    }
}
