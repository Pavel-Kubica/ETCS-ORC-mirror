/** @file BtmService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service representing the BTM component of ETCS system
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once


#include "BaliseDataService.hpp"
#include "JRULoggerService.hpp"
#include "MqttPublisherService.hpp"
#include "IBtmService.hpp"
#include "BaliseCoordinatesPrinterService.hpp"

class BtmService: public IBtmService, public IInitializable {
public:
    void Initialize(ServiceContainer& container) override;

    void CheckIfBaliseWasPassed(Distance previousDistance, Distance currentDistance) override;

private:

    void SendMessageThatBaliseWasPassed(const Balise& balise);

    IBaliseDataService* baliseDataService;
    IMqttPublisherService* mqttPublisherService;
    JRULoggerService* jruLoggerService;
    IBaliseCoordinatesPrinterService* printerService;
};
