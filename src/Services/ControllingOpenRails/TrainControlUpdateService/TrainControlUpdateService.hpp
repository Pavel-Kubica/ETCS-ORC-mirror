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
#include "CabControlApiService.hpp"
#include "MachineControlDataService.hpp"
#include "FromTIUMessage.hpp"
#include "MqttPublisherService.hpp"
#include "ILocalCabControlsDataService.hpp"
#include "IHumanControlDataService.hpp"
#include "OpenRailsTrainBrakeConfiguration.hpp"
#include "JRULoggerService.hpp"

class TrainControlUpdateService : public ITrainControlUpdateService,
                                  public ILpcManageable,
                                  public IInitializable {
protected:
    void Initialize(ServiceContainer& container) override;

public:
    void Update() override;
    
    // LPC manageable so we reload train brake config every start/restart
    bool LpcSaidStart() override;
    bool LpcSaidStop() override;
    bool LpcSaidRestart() override;
    
private:
    ICabControlApiService* cabControlApiService;
    IHumanControlDataService* humanControlDataService;
    IMachineControlDataService* machineControlDataService;
    IMqttPublisherService* mqttPublisherService;
    IIncrementalCabControlService* incrementalCabControlService;
    ILocalCabControlsDataService* openRailsState;
    JRULoggerService* jruLoggerService;
    
    ConfigurationService* configurationService;
    OpenRailsTrainBrakeConfiguration trainBrakeConfig;
    
    void SendFromTiuMessageToEvc();
    
    void SendOpenRailsCabControlsRequest();
    
    /**
     * Handles the state of `this->machineControlDataService`.
     * @return True if some handling was necessary, false otherwise.
     * If (and only if) this function returns false, the state of the `this->humanControlDataService`
     * should be handled.
     */
    bool HandleMachineInstructions();
    
    /**
     * Handles the state of `this->humanControlDataService`.
     */
    void HandleHumanInstructions();
    void HandleEngineBrake(CabControlRequest& request);
    void HandleDirectionLever(CabControlRequest &request);
    void HandleAuxiliaryFunctions(CabControlRequest &request);
    /**
     * This function might need to send requests in a specific order, so it doesn't take the request as an argument
     */
    void HandleDrivingLever();

    bool ReverserNotNeutral();
};