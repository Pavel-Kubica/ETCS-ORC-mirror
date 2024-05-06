/** @file IOdoToEvcSenderService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for Odo to Evc sending service
 *
 *  ###Contributors
 *  nguyem12
 */

#pragma once

#include "ISimulationStateSender.hpp"
#include "IService.hpp"

class IOdoToEvcSenderService : public ISimulationStateSender,
                               public IService {
public:
    static constexpr ServiceType Type = ServiceType::OdoToEvcSender;
};