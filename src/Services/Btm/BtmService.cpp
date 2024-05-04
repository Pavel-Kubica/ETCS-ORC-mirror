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


#include "BtmService.hpp"

void BtmService::Initialize(ServiceContainer& container) {
    baliseDataService = container.FetchService<BaliseDataService>().get();
    mqttPublisherService = container.FetchService<MqttPublisherService>().get();
}

void BtmService::CheckIfBaliseWasPassed(Distance previousDistance, Distance currentDistance) {
    if (previousDistance == currentDistance) {
        return;
    }
    const std::vector<std::unique_ptr<Balise>>& balises = baliseDataService->GetBalisesSortedByDistance();
    // finds the first balise which has higher position than previousDistance
    auto baliseIter = std::upper_bound(balises.begin(), balises.end(), previousDistance,
                                       [](Distance distance, const std::unique_ptr<Balise>& balise) {
                                            return distance < balise->GetPos();
                                       });
    if (baliseIter == balises.end()) {
        return;
    }
    for (; (*baliseIter)->GetPos() <= currentDistance; ++baliseIter) {
        SendMessageThatBaliseWasPassed(**baliseIter);
    }

}

void BtmService::SendMessageThatBaliseWasPassed(const Balise& balise) {

}
