/** @file SimulationState.h
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Basically a response from /API/ORC endpoint of Open Rails API.
 *  Structure describing the state of Open Rails simulation.
 *
 *  ###Contributors
 *  pavlian5
 *  rehorja8
 */

#pragma once

#include <string>

struct SimulationState {
    double distanceTravelledInMetres;
    double speedInMetresPerSecond;
    double accelerationInMetersPerSecondSquared;
    double throttleLeverPercentage;
    double direction;
    double motiveForceInNewtons;
    double brakeCylinderPressureInPsi;
    double mainPipeBrakePressureInPsi;
    double mainReservoirPressureInPsi;
    std::string worldLocationString;
    std::string worldPositionString;
};
