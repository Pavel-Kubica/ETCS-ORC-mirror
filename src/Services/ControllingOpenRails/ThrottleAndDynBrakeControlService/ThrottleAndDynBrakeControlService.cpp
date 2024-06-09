/** @file ThrottleAndDynBrakeControlService.cpp
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

#include "ThrottleAndDynBrakeControlService.hpp"
#include "CabControlRequest.hpp"
#include <stdexcept>

void ThrottleAndDynBrakeControlService::Initialize(ServiceContainer& container) {
    cabControlApiService = container.FetchService<ICabControlApiService>().get();
    localCabControlsDataService = container.FetchService<ILocalCabControlsDataService>().get();
    configurationService = container.FetchService<ConfigurationService>().get();
    
    this->config = configurationService->FetchConfiguration<ThrottleAndDynBrakeControlConfiguration>();
    
    localCabControlsDataService->SetThrottleStep(config.throttleStep);
    localCabControlsDataService->SetDynamicBrakeStep(config.brakeStep);
}

void ThrottleAndDynBrakeControlService::StartIncreasingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::Positive;
    throttleWasSet = false;
    cv.notify_one();
}

void ThrottleAndDynBrakeControlService::StopChangingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::None;
    throttleWasSet = false;
    cv.notify_one();
}

void ThrottleAndDynBrakeControlService::StartDecreasingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::Negative;
    throttleWasSet = false;
    cv.notify_one();
}

void ThrottleAndDynBrakeControlService::StartIncreasingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    brakeIncrement = Increment::Positive;
    dynamicBrakeWasSet = false;
    cv.notify_one();
}

void ThrottleAndDynBrakeControlService::StopChangingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    brakeIncrement = Increment::None;
    dynamicBrakeWasSet = false;
    cv.notify_one();
}

void ThrottleAndDynBrakeControlService::StartDecreasingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    brakeIncrement = Increment::Negative;
    dynamicBrakeWasSet = false;
    cv.notify_one();
}

bool ThrottleAndDynBrakeControlService::LpcSaidStart() {
    if (shouldRun) {
        return false;
    }
    shouldRun = true;
    incrementingThread = std::thread(&ThrottleAndDynBrakeControlService::IncrementingThreadEntryPoint, this);
    return true;
}

bool ThrottleAndDynBrakeControlService::LpcSaidStop() {
    std::unique_lock lck(this->mtx);
    shouldRun = false;
    cv.notify_one();
    lck.unlock();

    // `this->mtx` has to be unlocked when `.join()` is called
    incrementingThread.join();
    return true;
}

bool ThrottleAndDynBrakeControlService::LpcSaidRestart() {
    return LpcSaidStop() && LpcSaidStart();
}

bool ThrottleAndDynBrakeControlService::WorkDone() const {
    // No mutex since we call this method when this->mtx is locked
    return (throttleIncrement == Increment::None && brakeIncrement == Increment::None);
}

void ThrottleAndDynBrakeControlService::IncrementingThreadEntryPoint() {
    while (shouldRun) {
        CabControlRequest request;
        
        std::unique_lock lk(this->mtx);
        cv.wait(lk, [this]() { return !shouldRun || !WorkDone(); });
        
        this->ChangeThrottle(request);
        this->ChangeBrake(request);
        lk.unlock();
        
        cabControlApiService->Send(request);
        std::this_thread::sleep_for(config.timeBetweenIncrements);
    }
}

void ThrottleAndDynBrakeControlService::ChangeThrottle(CabControlRequest& request) {
    // No mutex since we call this method when this->mtx is locked
    if (this->throttleWasSet) {
        request.SetThrottle(this->localCabControlsDataService->GetThrottle());
        this->throttleWasSet = false;
        this->throttleIncrement = Increment::None;
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

void ThrottleAndDynBrakeControlService::ChangeBrake(CabControlRequest& request) {
    // No mutex since we call this method when this->mtx is locked
    if (this->dynamicBrakeWasSet) {
        request.SetDynamicBrake(this->localCabControlsDataService->GetDynamicBrake());
        this->dynamicBrakeWasSet = false;
        this->brakeIncrement = Increment::None;
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
void ThrottleAndDynBrakeControlService::SetThrottleTo(double value) {
    if (throttleIncrement == Increment::None) // If not, then the incrementing thread will do exactly this on its next tick anyway
    {
        CabControlRequest request;
        request.SetThrottle(value);
        cabControlApiService->Send(request);
    }
    std::lock_guard lk(this->mtx);
    this->localCabControlsDataService->SetThrottle(value);
    this->throttleWasSet = true;
}

void ThrottleAndDynBrakeControlService::SetDynamicBrakeTo(double value) {
    if (brakeIncrement == Increment::None) // If not, then the incrementing thread will do exactly this on its next tick anyway
    {
        CabControlRequest request;
        request.SetDynamicBrake(value);
        cabControlApiService->Send(request);
    }
    std::lock_guard lk(this->mtx);
    this->localCabControlsDataService->SetDynamicBrake(value);
    this->dynamicBrakeWasSet = true;
}
