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
 */

#include "ServiceContainer.hpp"
#include "IInitializable.hpp"
#include "IStartable.hpp"

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

void ServiceContainer::LpcSaidStart() {
    for (auto& service : lpcManageableServices) {
        service->LpcSaidStart();
    }
}

void ServiceContainer::LpcSaidStop() {
    for (auto& service : lpcManageableServices) {
        service->LpcSaidStop();
    }
}

void ServiceContainer::LpcSaidRestart() {
    for (auto& service : lpcManageableServices) {
        service->LpcSaidRestart();
    }
}
void ServiceContainer::AppExit() {
    LpcSaidStop();
    for(auto& service : startableServices){
        service->AppExit();
    }
}
