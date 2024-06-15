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

    localCabControlsDataService->SetThrottleIncStep(config.throttleIncStep);
    localCabControlsDataService->SetThrottleDecStep(config.throttleDecStep);
    localCabControlsDataService->SetDynamicBrakeIncStep(config.dynamicBrakeIncStep);
    localCabControlsDataService->SetDynamicBrakeDecStep(config.dynamicBrakeDecStep);
    localCabControlsDataService->SetEngineBrakeIncStep(config.engineBrakeIncStep);
    localCabControlsDataService->SetEngineBrakeDecStep(config.engineBrakeDecStep);
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
    dynamicBrakeIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    dynamicBrakeIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingDynamicBrake() {
    std::lock_guard lck(this->mtx);
    dynamicBrakeIncrement = Increment::Negative;
    cv.notify_one();
}

void IncrementalCabControlService::StartIncreasingEngineBrake() {
    std::lock_guard lck(this->mtx);
    engineBrakeIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingEngineBrake() {
    std::lock_guard lck(this->mtx);
    engineBrakeIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingEngineBrake() {
    std::lock_guard lck(this->mtx);
    engineBrakeIncrement = Increment::Negative;
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
    return (throttleIncrement == Increment::None &&
            dynamicBrakeIncrement == Increment::None &&
            engineBrakeIncrement == Increment::None);
}

void IncrementalCabControlService::IncrementingThreadEntryPoint() {
    while (shouldRun) {
        CabControlRequest request;
        
        std::unique_lock lk(this->mtx);
        cv.wait(lk, [this]() { return !shouldRun || !WorkDone(); });
        
        this->ChangeThrottle(request);
        this->ChangeDynamicBrake(request);
        this->ChangeEngineBrake(request);
        cabControlApiService->Send(request);
        lk.unlock();

        std::this_thread::sleep_for(config.timeBetweenIncrements);
    }
}

void IncrementalCabControlService::ChangeThrottle(CabControlRequest& request) {
    // No mutex since we call this method when this->mtx is locked
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

void IncrementalCabControlService::ChangeDynamicBrake(CabControlRequest& request) {
    // No mutex since we call this method when this->mtx is locked
    switch (dynamicBrakeIncrement) {
        case Increment::Positive: {
            bool finished = !localCabControlsDataService->IncreaseDynamicBrake();
            request.SetDynamicBrake(localCabControlsDataService->GetDynamicBrake());
            if (finished)
                dynamicBrakeIncrement = Increment::None;
            break;
        }
        case Increment::Negative: {
            bool finished = !localCabControlsDataService->DecreaseDynamicBrake();
            request.SetDynamicBrake(localCabControlsDataService->GetDynamicBrake());
            if (finished)
                dynamicBrakeIncrement = Increment::None;
            break;
        }
        case Increment::None:
            break;
        default:
            throw std::runtime_error("unreachable branch");
    }
}

void IncrementalCabControlService::ChangeEngineBrake(CabControlRequest &request) {
    // No mutex since we call this method when this->mtx is locked
    switch (engineBrakeIncrement) {
        case Increment::Positive: {
            bool finished = !localCabControlsDataService->IncreaseEngineBrake();
            request.SetEngineBrake(localCabControlsDataService->GetEngineBrake());
            if (finished)
                throttleIncrement = Increment::None;
            break;
        }
        case Increment::Negative: {
            bool finished = !localCabControlsDataService->DecreaseEngineBrake();
            request.SetEngineBrake(localCabControlsDataService->GetEngineBrake());
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

void IncrementalCabControlService::SetThrottleTo(double value) {
    std::lock_guard lk(this->mtx);
    localCabControlsDataService->SetThrottle(value);
    CabControlRequest request;
    request.SetThrottle(value);
    cabControlApiService->Send(request);
    throttleIncrement = Increment::None;
}

void IncrementalCabControlService::SetDynamicBrakeTo(double value) {
    std::lock_guard lk(this->mtx);
    localCabControlsDataService->SetDynamicBrake(value);
    CabControlRequest request;
    request.SetDynamicBrake(value);
    cabControlApiService->Send(request);
    dynamicBrakeIncrement = Increment::None;
}

void IncrementalCabControlService::SetEngineBrakeTo(double value) {
    std::lock_guard lk(this->mtx);
    localCabControlsDataService->SetEngineBrake(value);
    CabControlRequest request;
    request.SetEngineBrake(value);
    cabControlApiService->Send(request);
    engineBrakeIncrement = Increment::None;
}

