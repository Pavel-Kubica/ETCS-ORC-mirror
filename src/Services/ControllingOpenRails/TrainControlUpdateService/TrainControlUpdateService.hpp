/** @file HumanControlDataService.hpp
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
#include "../../../cem/Messages/Tiu/Evc/FromTIUMessage.hpp"
#include "MqttPublisherService.hpp"
#include "ILocalCabControlsDataService.hpp"
#include "IHumanControlDataService.hpp"

class TrainControlUpdateService : public ITrainControlUpdateService,
                                  public IInitializable {
protected:
    void Initialize(ServiceContainer& container) override;

public:
    void Update() override;

private:
    ICabControlApiService* cabControlApiService;
    IHumanControlDataService* humanControlDataService;
    IMachineControlDataService* machineControlDataService;
    IMqttPublisherService* mqttPublisherService;
    IIncrementalCabControlService* incrementApiService;
    ILocalCabControlsDataService* openRailsState;
    
    void SendFromTiuMessageToEvc();
    
    void SendOpenRailsCabControlsRequest();
    
    void SetThrottleInternal(double value, CabControlRequest& request);
    
    void SetEngineBrakeInternal(double value, CabControlRequest& request);
    
    /**
     * Handles the state of `this->machineControlDataService`.
     * @return True if some handling was necessary, false otherwise.
     * If (and only if) this function returns false, the state of the `this->humanControlDataService`
     * should be handled.
     */
    bool HandleMachineInstructions(CabControlRequest & request);
    
    /**
     * Handles the state of `this->humanControlDataService`.
     */
    void HandleHumanInstructions(CabControlRequest & request);
    
};