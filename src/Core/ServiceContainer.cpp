/** @file ServiceContainer.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Wrapper for all services
 *
 *  ### Contributors
 *  spakoro1
 *  veselo21
 *  zimaluk1
 *  pavlian5
 */

#include "ServiceContainer.hpp"
#include "IInitializable.hpp"
#include "IStartable.hpp"

#include <stdexcept>

void ServiceContainer::StartServices() {
    for (auto& startable : startableServices) {
        startable->Start(*this);
    }
}

void ServiceContainer::WaitForServices() {
    for (auto& startable : startableServices) {
        startable->Wait();
    }
}

void ServiceContainer::InitializeServices() {
    for (auto& initializable : initializableServices) {
        initializable->Initialize(*this);
    }
}

bool ServiceContainer::LpcSaidStart() {
    std::vector<std::shared_ptr<ILpcManageable>> started;

    for (auto& service : lpcManageableServices) {
        if(!service->LpcSaidStart()) {
            for(auto& service : started) {
                if(!service->LpcSaidStop()) {
                    //If we reach this place, we are fu**ed
                    throw std::runtime_error("During LPC start something went wrong and already stared services could not stop");
                }
            }
            return false;
        }
        started.push_back(service);
    }

    return true;
}

bool ServiceContainer::LpcSaidStop() {
    for (auto& service : lpcManageableServices) {
        if(!service->LpcSaidStop()){
            return false;
        }
    }

    return true;
}

bool ServiceContainer::LpcSaidRestart() {
    for (auto& service : lpcManageableServices) {
        if(!service->LpcSaidRestart()){
            return false;
        }
    }
    return true;
}
void ServiceContainer::AppExit() {
    LpcSaidStop();
    for(auto& service : startableServices){
        service->AppExit();
    }
}
