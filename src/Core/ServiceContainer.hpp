/** @file ServiceContainer.hpp
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
 *  maresj39
 */

#pragma once

#include <map>
#include <memory>
#include <vector>
#include "ILpcManagable.hpp"
#include "IService.hpp"

class IStartable;
class IInitializable;
/**
 * Container with all the services inside of it
 * Used as a sort of dependency injection tool
 * Every service uses it to get all the other services it needs to function
 */
class ServiceContainer {
public:
    /**
     * Returns a fully functional service of type T
     * @tparam T type of the service requested
     * @return requested service
     */
    template <typename T>
    std::shared_ptr<T> FetchService() {
        return std::dynamic_pointer_cast<T>(services[T::Type]);
    }
    /**
     * Lets all the lpcManageable services know that LPC sent a start message
     */
    void LpcSaidStart();
    /**
     * Lets all the lpcManageable services know that LPC sent a stop message
     */
    void LpcSaidStop();
    /**
     * Lets all the lpcManageable services know that LPC sent a restart message
     */
    void LpcSaidRestart();

protected:
    /**
     * Lets all the initializable services know to initialize
     */
    void InitializeServices();
    /**
     * Lets all the startable services know to start
     */
    void StartServices();
    /**
     * Lets all the startable services know to wait
     */
    void WaitForServices();
    /**
     * Registers a new service of type T = Constructs it and stores it into the corresponding vectors
     * -> startable/initializable/lpcManageable Services and also adds it to the general services map
     * @tparam T type of new Service
     */
    void AppExit();

    template <class T>
    void RegisterService() {
        std::shared_ptr<T> service = std::make_shared<T>();
        if (!service)
            return;
        std::shared_ptr<IService> existing = FetchService<T>();
        if (existing)
            return;
        services[T::Type] = service;
        std::shared_ptr<IStartable> startable = std::dynamic_pointer_cast<IStartable>(service);
        if (startable)
            startableServices.push_back(startable);
        std::shared_ptr<IInitializable> initializable = std::dynamic_pointer_cast<IInitializable>(service);
        if (initializable)
            initializableServices.push_back(initializable);
        std::shared_ptr<ILpcManagable> lpcManagable = std::dynamic_pointer_cast<ILpcManagable>(service);
        if (lpcManagable)
            lpcManagableServices.push_back(lpcManagable);
    }

    friend class Application;
    friend class ConsoleInputService;

    std::map<ServiceType, std::shared_ptr<IService>> services;
    std::vector<std::shared_ptr<IStartable>> startableServices;
    std::vector<std::shared_ptr<IInitializable>> initializableServices;
    std::vector<std::shared_ptr<ILpcManagable>> lpcManagableServices;
};
