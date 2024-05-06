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

void TrainControlUpdateService::Update() {

}

void TrainControlUpdateService::SendFromTiuMessageToEvc() {

}

void TrainControlUpdateService::SendOpenRailsCabControlsRequest() {

}

void TrainControlUpdateService::Initialize(ServiceContainer &container) {
    cabControlApiService = container.FetchService<CabControlApiService>().get();
    trainControlDataService = container.FetchService<TrainControlDataService>().get();
    machineControlDataService = container.FetchService<MachineControlDataService>().get();
}
