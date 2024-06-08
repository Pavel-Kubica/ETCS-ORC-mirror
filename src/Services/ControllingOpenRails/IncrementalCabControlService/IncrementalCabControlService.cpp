/** @file IncrementalCabControlService.cpp
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

#include "IncrementalCabControlService.hpp"
#include "CabControlRequest.hpp"
#include <stdexcept>

void IncrementalCabControlService::Initialize(ServiceContainer& container) {
    cabControlApiService = container.FetchService<ICabControlApiService>().get();
    localCabControlsDataService = container.FetchService<ILocalCabControlsDataService>().get();
    configurationService = container.FetchService<ConfigurationService>().get();
    
    this->config = configurationService->FetchConfiguration<IncrementalCabControlConfiguration>();
    
    localCabControlsDataService->SetThrottleStep(config.throttleStep);
    localCabControlsDataService->SetDynamicBrakeStep(config.brakeStep);
}

void IncrementalCabControlService::StartIncreasingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::Negative;
    cv.notify_one();
}

void IncrementalCabControlService::StartIncreasingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    brakeIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    brakeIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    brakeIncrement = Increment::Negative;
    cv.notify_one();
}

bool IncrementalCabControlService::LpcSaidStart() {
    if (shouldRun) {
        return false;
    }
    shouldRun = true;
    incrementingThread = std::thread(&IncrementalCabControlService::IncrementingThreadEntryPoint, this);
    return true;
}

bool IncrementalCabControlService::LpcSaidStop() {
    std::unique_lock lck(this->mtx);
    shouldRun = false;
    cv.notify_one();
    lck.unlock();

    // `this->mtx` has to be unlocked when `.join()` is called
    incrementingThread.join();
    return true;
}

bool IncrementalCabControlService::LpcSaidRestart() {
    return LpcSaidStop() && LpcSaidStart();
}

bool IncrementalCabControlService::WorkDone() const {
    // No mutex since we call this method when this->mtx is locked
    return (throttleIncrement == Increment::None && brakeIncrement == Increment::None);
}

void IncrementalCabControlService::IncrementingThreadEntryPoint() {
    while (shouldRun) {
        CabControlRequest request;
        
        std::unique_lock lk(this->mtx);
        cv.wait(lk, [this]() { return !shouldRun || !WorkDone(); });
        
        this->ChangeThrottle(request);
        if (localCabControlsDataService->GetThrottle() == 0) { // If we attempt to change brake when throttle is not zero, local controls data will be desynchronized
            this->ChangeBrake(request);
        }
        lk.unlock();
        
        cabControlApiService->Send(request);
        std::this_thread::sleep_for(config.timeBetweenIncrements);
    }
}

void IncrementalCabControlService::ChangeThrottle(CabControlRequest& request) {
    // No mutex since we call this method when this->mtx is locked
    if (this->throttleWasSet) {
        request.SetThrottle(this->localCabControlsDataService->GetThrottle());
        this->throttleIncrement = Increment::None;
        this->throttleWasSet = false;
        return;
    }
    
    switch (throttleIncrement) {
        case Increment::Positive: {
            bool finished = !localCabControlsDataService->IncreaseThrottle();
            request.SetThrottle(localCabControlsDataService->GetThrottle());
            if (finished)
                throttleIncrement = Increment::None;
            break;
        }
        case Increment::Negative: {
            bool finished = !localCabControlsDataService->DecreaseThrottle();
            request.SetThrottle(localCabControlsDataService->GetThrottle());
            if (finished)
                throttleIncrement = Increment::None;
            break;
        }
        case Increment::None:
            break;
        default:
            throw std::runtime_error("unreachable branch");
    }
}

void IncrementalCabControlService::ChangeBrake(CabControlRequest& request) {
    // No mutex since we call this method when this->mtx is locked
    if (this->dynamicBrakeWasSet) {
        request.SetDynamicBrake(this->localCabControlsDataService->GetDynamicBrake());
        this->brakeIncrement = Increment::None;
        this->dynamicBrakeWasSet = false;
        return;
    }
    
    switch (brakeIncrement) {
        case Increment::Positive: {
            bool finished = !localCabControlsDataService->IncreaseDynamicBrake();
            request.SetDynamicBrake(localCabControlsDataService->GetDynamicBrake());
            if (finished)
                brakeIncrement = Increment::None;
            break;
        }
        case Increment::Negative: {
            bool finished = !localCabControlsDataService->DecreaseDynamicBrake();
            request.SetDynamicBrake(localCabControlsDataService->GetDynamicBrake());
            if (finished)
                brakeIncrement = Increment::None;
            break;
        }
        case Increment::None:
            break;
        default:
            throw std::runtime_error("unreachable branch");
    }
}
void IncrementalCabControlService::SetThrottleTo(double value) {
    std::lock_guard lck(this->mtx);
    this->localCabControlsDataService->SetThrottle(value);
    this->throttleWasSet = true;
}

void IncrementalCabControlService::SetDynamicBrakeTo(double value) {
    std::lock_guard lck(this->mtx);
    this->localCabControlsDataService->SetDynamicBrake(value);
    this->dynamicBrakeWasSet = true;
}
