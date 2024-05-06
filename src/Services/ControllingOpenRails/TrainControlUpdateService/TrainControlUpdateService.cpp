/** @file ITrainControlDataService.hpp
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
 */

#include "TrainControlUpdateService.hpp"
#include "ServiceContainer.hpp"

void TrainControlUpdateService::Initialize(ServiceContainer &container) {
    cabControlApiService = container.FetchService<CabControlApiService>().get();
    trainControlDataService = container.FetchService<TrainControlDataService>().get();
    machineControlDataService = container.FetchService<MachineControlDataService>().get();
    mqttPublisherService = container.FetchService<MqttPublisherService>().get();
}

void TrainControlUpdateService::Update() {
    SendFromTiuMessageToEvc();
    SendOpenRailsCabControlsRequest();
}

void TrainControlUpdateService::SendFromTiuMessageToEvc() {
    FromTIUMessage message{trainControlDataService->getBattery(), trainControlDataService->getCab(),
                           static_cast<Direction>(trainControlDataService->getTrainDirection())};
    mqttPublisherService->Publish(std::make_shared<FromTIUMessage>(message));
}

void TrainControlUpdateService::SendOpenRailsCabControlsRequest() {
    if (!trainControlDataService->getBattery() || !trainControlDataService->getCab()) return; // Cannot control the train if we are switched off
    cabControlApiService->SetDirection(trainControlDataService->getTrainDirection());

    // TODO interpret brake/throttle positions from DrivingLeverPosition

    cabControlApiService->SendAndClear();
}

