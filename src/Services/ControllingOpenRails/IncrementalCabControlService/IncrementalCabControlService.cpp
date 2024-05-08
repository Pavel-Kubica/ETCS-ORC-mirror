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
 */

#include "IncrementalCabControlService.hpp"
#include <stdexcept>

void IncrementalCabControlService::Initialize(ServiceContainer &container) {
    cabControlApiService = container.FetchService<ICabControlApiService>().get();
    localCabControlsDataService = container.FetchService<ILocalCabControlsDataService>().get();
    localCabControlsDataService->SetThrottleStep(THROTTLE_STEP);
    localCabControlsDataService->SetEngineBrakeStep(BRAKE_STEP);
}
void IncrementalCabControlService::StartIncreasingThrottle() {

}

void IncrementalCabControlService::StopChangingThrottle() {

}

void IncrementalCabControlService::StartDecreasingThrottle() {

}

void IncrementalCabControlService::StartIncreasingEngineBrake() {

}

void IncrementalCabControlService::StopChangingEngineBrake() {

}

void IncrementalCabControlService::StartDecreasingEngineBrake() {

}

bool IncrementalCabControlService::LpcSaidStart() {
    shouldRun = true;
    incrementingThread = std::thread(&IncrementalCabControlService::DoChanges, this);
    return true;
}

bool IncrementalCabControlService::LpcSaidStop() {
    shouldRun = false;
    incrementingThread.join();
    return true;
}

bool IncrementalCabControlService::LpcSaidRestart() {
    LpcSaidStop();
    LpcSaidStart();
}

// Returns true if incrementing thread has no more work to do
bool IncrementalCabControlService::WorkDone() const {
    return throttleIncrement == Increment::None && brakeIncrement == Increment::None;
}

void IncrementalCabControlService::DoChanges() {
    while (shouldRun) {
        std::unique_lock lk(mtx);
        cv.wait(lk);
        while (!WorkDone())
        {
            ChangeThrottle();
            ChangeBrake();
        }
    }
}

void IncrementalCabControlService::ChangeThrottle() {
    switch (throttleIncrement) {
        case Increment::Positive: {
            bool finished = !localCabControlsDataService->IncreaseThrottle();
            if (finished)
                throttleIncrement = Increment::None;
            break;
        }
        case Increment::Negative: {
            bool finished = !localCabControlsDataService->DecreaseThrottle();
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

void IncrementalCabControlService::ChangeBrake() {
    switch (brakeIncrement) {
        case Increment::Positive: {
            bool finished = !localCabControlsDataService->IncreaseEngineBrake();
            if (finished)
                brakeIncrement = Increment::None;
            break;
        }
        case Increment::Negative: {
            bool finished = !localCabControlsDataService->DecreaseEngineBrake();
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
