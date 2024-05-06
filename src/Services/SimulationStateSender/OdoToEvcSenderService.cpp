/** @file OdoToEvcSenderService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Service for sending Odo messages to Evc
 *
 *  ###Contributors
 *  nguyem12
 */


#include "OdoToEvcSenderService.hpp"
#include "Odo/Evc/ODOMeasurementsMessage.hpp"


void OdoToEvcSenderService::Initialize(ServiceContainer &container) {
    this->mqttPublisher = container.FetchService<IMqttPublisherService>().get();
}

void OdoToEvcSenderService::SendSimulationState(const SimulationState& simulationState) {
    double speed = simulationState.speedInMetresPerSecond;
    ODODirection q_control;

    if (speed > 0) {
        q_control = ODODirection::Forward;
    } else if (speed < 0) {
        q_control = ODODirection::Reverse;
    } else {
        q_control = ODODirection::Stationary;
    }

    std::shared_ptr<ODOMeasurementsMessage> odoMeasurements = std::make_shared<ODOMeasurementsMessage>(
        Q_CONTROL,
        static_cast<uint32_t>(simulationState.distanceTravelledInMetres),
        simulationState.speedInMetresPerSecond,
        static_cast<uint16_t>(simulationState.accelerationInMetersPerSecondSquared),
        NID_C,
        NID_BG,
        DL_DOUBTOVER,
        DL_DOUBTUNDER,
        V_DOUBTPOS,
        V_DOUBTNEG,
        q_control
    );

    mqttPublisher->Publish(odoMeasurements);
}