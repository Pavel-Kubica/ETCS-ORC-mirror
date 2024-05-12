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
#include "ODODirection.hpp"


void OdoToEvcSenderService::Initialize(ServiceContainer &container) {
    mqttPublisher = container.FetchService<IMqttPublisherService>().get();
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

    ODOMeasurementsMessage odoMeasurements(
        Q_CONTROL,
        static_cast<uint32_t>(simulationState.distanceTravelledInMetres),
        abs(simulationState.speedInMetresPerSecond) * 3.6,
        static_cast<uint16_t>(simulationState.accelerationInMetersPerSecondSquared),
        NID_C,
        NID_BG,
        DL_DOUBTOVER,
        DL_DOUBTUNDER,
        V_DOUBTPOS,
        V_DOUBTNEG,
        q_control
    );

    mqttPublisher->Publish(std::make_shared<ODOMeasurementsMessage>(odoMeasurements));
}