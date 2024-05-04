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
#include "Btm/Evc/BTMMessage.hpp"
#include "EndOfInformation.hpp"

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
    auto baliseIter = std::lower_bound(balises.begin(), balises.end(), previousDistance,
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
    BaliseDirection qUpdown = balise.GetTelegram().Q_UPDOWN == 0 ? BaliseDirection::Reverse : BaliseDirection::Nominal;
    std::vector<std::shared_ptr<Packet>> packets = {std::make_shared<Packet>(EndOfInformation())};
    std::shared_ptr<BTMMessage> message = std::make_shared<BTMMessage>(
            balise.GetTelegram().Q_MEDIA,
            balise.GetTelegram().M_VERSION,
            qUpdown,
            balise.GetTelegram().N_TOTAL,
            balise.GetTelegram().M_MCOUNT,
            balise.GetTelegram().M_DUP,
            balise.GetTelegram().NID_C,
            balise.GetTelegram().N_PIG,
            balise.GetTelegram().Q_LINK,
            balise.GetTelegram().NID_BG,
            packets
    );
    mqttPublisherService->Publish(message);
}
