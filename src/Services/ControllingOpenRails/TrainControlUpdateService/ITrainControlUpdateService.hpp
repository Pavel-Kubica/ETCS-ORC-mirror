/** @file ITrainControlDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for TrainControlUpdateService
 *
 *  ###Contributors
 *  kubicpa3
 */

#pragma once

#include "IService.hpp"

class ITrainControlUpdateService : public IService {
public:
    virtual void Update() = 0;

    static constexpr ServiceType Type = ServiceType::TrainControlUpdate;
};