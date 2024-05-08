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
    throttleIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingThrottle() {
    throttleIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingThrottle() {
    throttleIncrement = Increment::Negative;
    cv.notify_one();
}

void IncrementalCabControlService::StartIncreasingEngineBrake() {
    brakeIncrement = Increment::Positive;
    cv.notify_one();
}

void IncrementalCabControlService::StopChangingEngineBrake() {
    brakeIncrement = Increment::None;
    cv.notify_one();
}

void IncrementalCabControlService::StartDecreasingEngineBrake() {
    brakeIncrement = Increment::Negative;
    cv.notify_one();
}

bool IncrementalCabControlService::LpcSaidStart() {
    shouldRun = true;
    incrementingThread = std::thread(&IncrementalCabControlService::DoChanges, this);
    return true;
}

bool IncrementalCabControlService::LpcSaidStop() {
    shouldRun = false;
    cv.notify_one();
    incrementingThread.join();
    return true;
}

bool IncrementalCabControlService::LpcSaidRestart() {
    return LpcSaidStop() && LpcSaidStart();
}

// Returns true if incrementing thread has no more work to do
bool IncrementalCabControlService::WorkDone() const {
    return (throttleIncrement == Increment::None && brakeIncrement == Increment::None)
           || !shouldRun;
}

void IncrementalCabControlService::DoChanges() {
    while (shouldRun) {
        std::unique_lock lk(mtx);
        cv.wait(lk);
        while (!WorkDone()) {
            CabControlRequest request;
            ChangeThrottle(request);
            ChangeBrake(request);
            cabControlApiService->Send(request);
            std::this_thread::sleep_for(TIMEOUT_BETWEEN_INCREMENTS);
        }
    }
}

void IncrementalCabControlService::ChangeThrottle(CabControlRequest& request) {
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
