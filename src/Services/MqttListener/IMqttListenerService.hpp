/** @file IMqttListenerService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for MqttListenerService
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 */

#pragma once

 #include "IService.hpp"
/**
 * Interface that is responsible for listening and giving workOrders to topic workers
 */
class IMqttListenerService : public IService {
public:
    explicit IMqttListenerService() : IService() {}

    virtual bool GetStarted() const = 0;

    static constexpr ServiceType Type = ServiceType::Listener;
};
