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
#include "IInitializable.hpp"
#include "ILpcManageable.hpp"
#include "ILocalCabControlsDataService.hpp"
#include "ICabControlApiService.hpp"

#include "AsyncProperty.hpp"
#include "Increment.hpp"
#include <chrono>
#include <atomic>
#include <thread>
#include <condition_variable>

class IncrementalCabControlService : public IInitializable,
                                     public ILpcManageable,
                                     public IIncrementalCabControlService {
private:
    // TODO load all of these from config?
    static constexpr double THROTTLE_STEP = 0.02;
    static constexpr double BRAKE_STEP = 0.02;
    static constexpr std::chrono::duration TIMEOUT_BETWEEN_INCREMENTS = std::chrono::milliseconds(100);

protected:
    void Initialize(ServiceContainer& container) override;
public:
    void StartIncreasingThrottle() override;
    void StopChangingThrottle() override;
    void StartDecreasingThrottle() override;
    
    void StartIncreasingEngineBrake() override;
    void StopChangingEngineBrake() override;
    void StartDecreasingEngineBrake() override;
    
    bool LpcSaidStart() override;
    
    bool LpcSaidStop() override;
    
    bool LpcSaidRestart() override;


private:
    ICabControlApiService* cabControlApiService;
    ILocalCabControlsDataService* localCabControlsDataService;
    
    std::atomic<Increment> throttleIncrement;
    std::atomic<Increment> brakeIncrement;
    
    std::atomic_bool shouldRun;
    std::thread incrementingThread;
    std::mutex mtx;
    std::condition_variable cv;
    
    void DoChanges();
    void ChangeThrottle(CabControlRequest& request);
    void ChangeBrake(CabControlRequest& request);
    [[nodiscard]] bool WorkDone() const;
    
};