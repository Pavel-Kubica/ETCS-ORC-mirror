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
    throttleAndDynBrakeService = container.FetchService<IThrottleAndDynBrakeControlService>().get();
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

    bool hadToHandleMachineInstructions = this->HandleMachineInstructions();
    if (!hadToHandleMachineInstructions) {
        this->HandleHumanInstructions();
    }
}

bool TrainControlUpdateService::HandleMachineInstructions() {
    CabControlRequest request;
    if (this->machineControlDataService->GetEmergencyBrake()) {
        this->throttleAndDynBrakeService->SetThrottleTo(0);
        request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::EMERGENCY));
        this->openRailsState->SetTrainBrake(TrainBrake::EMERGENCY);
        cabControlApiService->Send(request);
        return true;
    }

    if (this->machineControlDataService->GetServiceBrake()) {
        this->throttleAndDynBrakeService->SetThrottleTo(0);
        request.SetEngineBrake(1);
        request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL));
        cabControlApiService->Send(request);
        return true;
    }
    
    return false;
}

// INVARIANTS:
// We must always ensure that !(DynBrake > 0 && Throttle > 0)
// Additionally, we cannot move dynamic brake if the Reverser is in the Neutral position

void TrainControlUpdateService::HandleHumanInstructions() {
    CabControlRequest request;
    HandleDirectionLever(request);
    HandleAuxiliaryFunctions(request);
    HandleEngineBrake(request);
    cabControlApiService->Send(request);

    HandleDrivingLever();
}

void TrainControlUpdateService::HandleDirectionLever(CabControlRequest& request) {
    if (openRailsState->GetDynamicBrake() == 0) { // Cannot move Reverser while DynBrake is active
        request.SetDirection(humanControlDataService->GetTrainDirection());
    }
}

void TrainControlUpdateService::HandleAuxiliaryFunctions(CabControlRequest& request) {
    request.SetPantograph(humanControlDataService->GetPantograph());
    request.SetSander(humanControlDataService->GetSander());
    request.SetHorn(humanControlDataService->GetHorn());
    request.SetEmergencyBrake(humanControlDataService->GetEmergencyBrake());
    request.SetLight(humanControlDataService->GetLight());
}

void TrainControlUpdateService::HandleEngineBrake(CabControlRequest &request) {
    switch (humanControlDataService->GetEngineBrake()) {
        case EngineBrakeLeverPosition::FullRelease:
            throttleAndDynBrakeService->SetEngineBrakeTo(0);
            break;
        case EngineBrakeLeverPosition::Release:
            throttleAndDynBrakeService->StartDecreasingEngineBrake();
            break;
        case EngineBrakeLeverPosition::Neutral:
            throttleAndDynBrakeService->StopChangingEngineBrake();
            break;
        case EngineBrakeLeverPosition::Engage:
            throttleAndDynBrakeService->StartIncreasingEngineBrake();
            break;
        case EngineBrakeLeverPosition::FullPower:
            throttleAndDynBrakeService->SetEngineBrakeTo(1);
            break;
    }
}

void TrainControlUpdateService::HandleDrivingLever() {
    CabControlRequest request;
    switch (humanControlDataService->GetDrivingLever()) {
        case DrivingLeverPosition::Accelerate: {
            if (ReverserNotNeutral()) {
                throttleAndDynBrakeService->SetDynamicBrakeTo(0);                                           // DYNAMIC BRAKE
            }
            throttleAndDynBrakeService->StartIncreasingThrottle();                                // THROTTLE

            TrainBrake newTrainBrakeState = TrainBrake::RELEASE;                                    // TRAIN BRAKE
            if (humanControlDataService->GetQuickRelease())
                newTrainBrakeState = TrainBrake::QUICK_RELEASE;
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(newTrainBrakeState));
            openRailsState->SetTrainBrake(newTrainBrakeState);
            break;
        }
        case DrivingLeverPosition::Continue: {
            // As long as the invariants are preserved everywhere else, we do not need any checks here
            throttleAndDynBrakeService->StopChangingThrottle();                                   // THROTTLE
            throttleAndDynBrakeService->StartDecreasingDynamicBrake();                            // DYNAMIC BRAKE

            TrainBrake newTrainBrakeState = TrainBrake::RELEASE;                                    // TRAIN BRAKE
            if (humanControlDataService->GetQuickRelease())
                newTrainBrakeState = TrainBrake::QUICK_RELEASE;
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(newTrainBrakeState));
            openRailsState->SetTrainBrake(newTrainBrakeState);
            break;
        }
        case DrivingLeverPosition::Neutral:
            // As long as the invariants are preserved everywhere else, we do not need any checks here
            throttleAndDynBrakeService->StartDecreasingThrottle();   // THROTTLE
            throttleAndDynBrakeService->StopChangingDynamicBrake();  // DYNAMIC BRAKE
            if (humanControlDataService->HasTouchedRelease()) {     // TRAIN BRAKE
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::RELEASE));
                openRailsState->SetTrainBrake(TrainBrake::RELEASE);
            } else {
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL));
                openRailsState->SetTrainBrake(TrainBrake::NEUTRAL);
            }
            break;
        case DrivingLeverPosition::ElectrodynamicBrake:
            throttleAndDynBrakeService->SetThrottleTo(0);                         // THROTTLE
            if (ReverserNotNeutral()) {
                throttleAndDynBrakeService->StartIncreasingDynamicBrake();        // DYNAMIC BRAKE
            }
            if (humanControlDataService->HasTouchedRelease()) {        // TRAIN BRAKE
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::RELEASE));
                openRailsState->SetTrainBrake(TrainBrake::RELEASE);
            } else {
                request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::NEUTRAL));
                openRailsState->SetTrainBrake(TrainBrake::NEUTRAL);
            }
            break;
        case DrivingLeverPosition::PneumaticBrake:
            throttleAndDynBrakeService->SetThrottleTo(0);                                                           // THROTTLE
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::APPLY));  // TRAIN BRAKE
            if (ReverserNotNeutral()) {
                throttleAndDynBrakeService->StartIncreasingDynamicBrake();                               // DYNAMIC BRAKE
            }
            openRailsState->SetTrainBrake(TrainBrake::APPLY);
            break;
        case DrivingLeverPosition::QuickBrake:
            throttleAndDynBrakeService->SetThrottleTo(0);                                                             // THROTTLE
            if (ReverserNotNeutral()) {                                                                  // DYNAMIC BRAKE
                // If this is the first time we are controlling dynamic brake, API will not react to just sending DynBrake 1
                throttleAndDynBrakeService->SetDynamicBrakeTo(0.1);
                throttleAndDynBrakeService->SetDynamicBrakeTo(0.6);
                throttleAndDynBrakeService->SetDynamicBrakeTo(1);
            }
            request.SetTrainBrake(trainBrakeConfig.ConvertToRequestValue(TrainBrake::EMERGENCY));  // TRAIN BRAKE
            openRailsState->SetTrainBrake(TrainBrake::EMERGENCY);
            break;
    }
    cabControlApiService->Send(request);
}

bool TrainControlUpdateService::ReverserNotNeutral() {
    return humanControlDataService->GetTrainDirection() != DirectionLeverPosition::Neutral;
}
