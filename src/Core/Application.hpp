/** @file Application.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  The main application class
 *
 *  ### Contributors
 *  spakoro1
 *  veselo21
 *  moravp15
 *  zimaluk1
 */

#pragma once

#include <memory>
#include <string>
#include "ServiceContainer.hpp"

class Application {
public:
    /**
     * service_container registers/constructs all the services inside of it
     * @param mqttHostname host for the mqtt session
     * @param mqttPort port for the mqtt session
     */
    void Initialize(const std::string& mqttHostname, int mqttPort = 1883);
    /**
     * service_container initializes and starts the services, thus starting the app
     */
    void Run();

private:
    std::shared_ptr<ServiceContainer> service_container;
};
