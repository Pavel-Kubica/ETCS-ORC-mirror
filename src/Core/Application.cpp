/** @file Application.cpp
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
 *  pavlian5
 */
#include <string>
#include "Application.hpp"
#include "Configuration/ConfigurationService.hpp"
#include "MqttPublisher/MqttPublisherService.hpp"
#include "MessageHandlersService/MessageHandlersService.hpp"
#include "MqttListener/MqttListenerService.hpp"
#include "SimulationStateData/SimulationStateDataService.hpp"
#include "BaliseDataService.hpp"
#include "DatabaseService.hpp"
#include "BtmService.hpp"
#include "GuiSimulationStateSenderService.hpp"
#include "OdoToEvcSenderService.hpp"
#include "ControllingOpenRails/CabControlApiService/CabControlApiService.hpp"
#include "ControllingOpenRails/TrainControlUpdateService/TrainControlUpdateService.hpp"
#include "ControllingOpenRails/IncrementalCabControlService/IncrementalCabControlService.hpp"
#include "LocalCabControlsDataService.hpp"
#include "HumanControlDataService.hpp"
#include "BalisesFromJsonService.hpp"



#include <stdio.h> /* defines FILENAME_MAX */


#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

void Application::Initialize(const std::string& mqttHostname, int mqttPort) {
    mosquitto_lib_init();
    service_container = std::make_shared<ServiceContainer>();
    service_container->RegisterService<ConfigurationService>();
    service_container->RegisterService<JRULoggerService>();
    service_container->RegisterService<MqttPublisherService>();
    service_container->RegisterService<MessageHandlersService>();
    service_container->RegisterService<MqttListenerService>();
    service_container->RegisterService<SimulationStateDataService>();
    service_container->RegisterService<BaliseDataService>();
    service_container->RegisterService<BalisesFromJsonService>();
    service_container->RegisterService<BtmService>();
    service_container->RegisterService<GuiSimulationStateSenderService>();
    service_container->RegisterService<OdoToEvcSenderService>();
    // set mqtt ip and port from method arguments
    service_container->FetchService<MqttPublisherService>()->SetMqttAddress(mqttHostname, mqttPort);
    service_container->FetchService<MqttListenerService>()->SetMqttAddress(mqttHostname, mqttPort);
    
    // Services for controlling OpenRails
    service_container->RegisterService<HumanControlDataService>();
    service_container->RegisterService<MachineControlDataService>();
    service_container->RegisterService<LocalCabControlsDataService>();
    service_container->RegisterService<IncrementalCabControlService>();
    service_container->RegisterService<TrainControlUpdateService>();
    service_container->RegisterService<CabControlApiService>();
}

void Application::Run() {
    service_container->InitializeServices();
    service_container->StartServices();
    service_container->WaitForServices();
}
