/** @file SimulationStatusMessageHandler.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler handling start/stop/restart messages from LPC
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  hamaljan
 *  pavlian5
 */

#include "SimulationStatusMessageHandler.hpp"
#include <iostream>
 #include "OrcSimulationStatusMessage.hpp"

SimulationStatusMessageHandler::SimulationStatusMessageHandler(ServiceContainer& container)
    : MessageHandler(container), serviceContainer(container) {
    mqttListener = container.FetchService<IMqttListenerService>().get();
    jruLoggerService = container.FetchService<JRULoggerService>().get();
}
void SimulationStatusMessageHandler::HandleMessageBody(const nlohmann::json& data) {
    // Try to get the message
    OrcSimulationStatusMessage msg;
    try {
        msg.from_json(data);
    } catch (nlohmann::json::out_of_range& e) {
        jruLoggerService->Log(true, MessageType::Error, "Message received was in wrong format. e: %e%", e.what());
        return;
    }

    // Log that the message has been received
    jruLoggerService->Log(true, MessageType::Note, "[LPC] ----> [ORC] |1150| Simulation status: %status%",
                          msg.GetSimulationStatus());

    // Handle START
    if (msg.GetSimulationStatus() == SimulationStatus::START) {
        if (mqttListener->GetStarted()) {
            jruLoggerService->Log(true, MessageType::Error, "ORC already received START message.");
            return;
        }
        // Start all the LpcManageable services
        serviceContainer.LpcSaidStart();
    } else if (msg.GetSimulationStatus() == SimulationStatus::STOP) {  // Handle STOP
        // Check if we were started before we stop
        if (!mqttListener->GetStarted()) {
            jruLoggerService->Log(true, MessageType::Error, "ORC was not started, aborting stop procedure");
            return;
        }
        // Stop all the LpcManageable services
        serviceContainer.LpcSaidStop();
    } else {
        serviceContainer.LpcSaidRestart();
    }
}
