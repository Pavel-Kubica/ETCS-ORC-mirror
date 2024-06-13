/** @file SimulationState.h
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Structure describing the state of Open Rails simulation
 *
 *  ###Contributors
 *  pavlian5
 */

#pragma once

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
};
