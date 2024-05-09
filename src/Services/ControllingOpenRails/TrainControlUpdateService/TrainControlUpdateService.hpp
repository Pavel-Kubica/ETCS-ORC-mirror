/** @file TrainControlDataService.hpp
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

#pragma once

#include "ITrainControlUpdateService.hpp"
#include "IIncrementalCabControlService.hpp"
#include "IInitializable.hpp"
#include "../CabControlApiService/CabControlApiService.hpp"
#include "../../MachineControlData/MachineControlDataService.hpp"
#include "../../TrainControlData/TrainControlDataService.hpp"
#include "../../../cem/Messages/Tiu/Evc/FromTIUMessage.hpp"
#include "MqttPublisherService.hpp"
#include "ILocalCabControlsDataService.hpp"

class TrainControlUpdateService : public ITrainControlUpdateService,
                                  public IInitializable {
protected:
    void Initialize(ServiceContainer& container) override;

public:
    void Update() override;

private:
    ICabControlApiService* cabControlApiService;
    ITrainControlDataService* trainControlDataService;
    IMachineControlDataService* machineControlDataService;
    IMqttPublisherService* mqttPublisherService;
    IIncrementalCabControlService* incrementApiService;
    ILocalCabControlsDataService* openRailsState;
    
    void SendFromTiuMessageToEvc();
    
    void SendOpenRailsCabControlsRequest();
    
    void SetThrottleInternal(double value, CabControlRequest& request);
    
    void SetEngineBrakeInternal(double value, CabControlRequest& request);
};