/** @file IncrementalCabControlService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  This service allows for incremental changing of throttle and engine brake
 *
 *  ###Contributors
 *  kubicpa3
 *  rehorja8
 */

#pragma once

#include "IIncrementalCabControlService.hpp"
#include "ConfigurationService.hpp"
#include "IInitializable.hpp"
#include "ILpcManageable.hpp"
#include "ILocalCabControlsDataService.hpp"
#include "ICabControlApiService.hpp"

#include "AsyncProperty.hpp"
#include "Increment.hpp"
#include "IncrementalCabControlConfiguration.hpp"
#include <chrono>
#include <atomic>
#include <thread>
#include <condition_variable>

class IncrementalCabControlService : public IInitializable,
                                     public ILpcManageable,
                                     public IIncrementalCabControlService {
protected:
    void Initialize(ServiceContainer& container) override;
public:
    void StartIncreasingThrottle() override;
    void StopChangingThrottle() override;
    void StartDecreasingThrottle() override;
    
    void StartIncreasingDynamicBrake() override;
    void StopChangingDynamicBrake() override;
    void StartDecreasingDynamicBrake() override;
    void SetThrottleTo(double value) override;
    void SetDynamicBrakeTo(double value) override;
    bool LpcSaidStart() override;
    
    bool LpcSaidStop() override;
    
    bool LpcSaidRestart() override;


private:
    ICabControlApiService* cabControlApiService;
    ILocalCabControlsDataService* localCabControlsDataService;
    
    Increment throttleIncrement;
    Increment brakeIncrement;
    
    // Loading from config files
    IncrementalCabControlConfiguration config;
    ConfigurationService* configurationService;
    
    // Says if the incrementing thread should be running or not
    std::atomic_bool shouldRun;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread incrementingThread;
    
    void IncrementingThreadEntryPoint();
    
    /**
     * @warning THIS METHOD IS NO SYNCHRONISED - IT DOES NOT LOCK `this->mtx`
     * @param [in, out] request - request that this method adds throttle changes to, which should be sent to Open Rails API
     */
    void ChangeThrottle(CabControlRequest& request);
    
    /**
     * @warning THIS METHOD IS NO SYNCHRONISED - IT DOES NOT LOCK `this->mtx`
     * @param [in, out] request - request that this method adds brake changes to, which should be sent to Open Rails API
     */
    void ChangeBrake(CabControlRequest& request);
    
    /**
     * @warning THIS METHOD IS NO SYNCHRONISED - IT DOES NOT LOCK `this->mtx`
     * @return true if incrementing thread has no more work to do
     * (meaning `this->throttleIncrement` and `this->brakeIncrement` are `Increment::None`).
     */
    [[nodiscard]] bool WorkDone() const;
    
};