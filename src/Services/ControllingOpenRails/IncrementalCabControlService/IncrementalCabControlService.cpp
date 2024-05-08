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
 */

#include "IncrementalCabControlService.hpp"
#include "CabControlRequest.hpp"
#include <stdexcept>

void IncrementalCabControlService::Initialize(ServiceContainer& container) {
    cabControlApiService = container.FetchService<ICabControlApiService>().get();
    localCabControlsDataService = container.FetchService<ILocalCabControlsDataService>().get();
    localCabControlsDataService->SetThrottleStep(THROTTLE_STEP);
    localCabControlsDataService->SetEngineBrakeStep(BRAKE_STEP);
}

void IncrementalCabControlService::StartIncreasingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingThrottle() {
    std::lock_guard lck(this->mtx); // TODO: this might not be here
    throttleIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingThrottle() {
    std::lock_guard lck(this->mtx);
    throttleIncrement = Increment::Negative;
    cv.notify_one();
}

void IncrementalCabControlService::StartIncreasingEngineBrake() {
    std::lock_guard lck(this->mtx);
    brakeIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingEngineBrake() {
    std::lock_guard lck(this->mtx); // TODO: this might not be here
    brakeIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingEngineBrake() {
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
    {
        std::lock_guard lck(this->mtx);
        shouldRun = false;
        cv.notify_one();
    }
    
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
        this->ChangeBrake(request);
        lk.unlock();
        
        cabControlApiService->Send(request);
        std::this_thread::sleep_for(TIMEOUT_BETWEEN_INCREMENTS);
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

void IncrementalCabControlService::ChangeBrake(CabControlRequest& request) {
    // No mutex since we call this method when this->mtx is locked
    switch (brakeIncrement) {
        case Increment::Positive: {
            bool finished = !localCabControlsDataService->IncreaseEngineBrake();
            request.SetEngineBrake(localCabControlsDataService->GetEngineBrake());
            if (finished)
                brakeIncrement = Increment::None;
            break;
        }
        case Increment::Negative: {
            bool finished = !localCabControlsDataService->DecreaseEngineBrake();
            request.SetEngineBrake(localCabControlsDataService->GetEngineBrake());
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
