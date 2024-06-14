/** @file BtmService.cpp
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
    this->baliseDataService = container.FetchService<BaliseDataService>().get();
    this->mqttPublisherService = container.FetchService<MqttPublisherService>().get();
    this->jruLoggerService = container.FetchService<JRULoggerService>().get();
    this->printerService = container.FetchService<IBaliseCoordinatesPrinterService>().get();
}

void BtmService::CheckIfBaliseWasPassed(Distance previousDistance, Distance currentDistance) {
    if (previousDistance == currentDistance) return;
    
    const std::vector<std::unique_ptr<Balise>>& balises = baliseDataService->GetBalisesSortedByDistance();
    uint32_t passedBalisesCnt = 0;

    if (previousDistance < currentDistance) {   // train is moving forwards
        //find the first balise which has higher position than previousDistance
        auto baliseIter = std::upper_bound(balises.begin(), balises.end(), previousDistance,
                                           [](Distance distance, const std::unique_ptr<Balise>& balise) {
                                               return distance < balise->GetPos();
                                           });
        // send a message for every balise with position in the interval (previousDistance, currentDistance>
        for (; baliseIter != balises.end() && (*baliseIter)->GetPos() <= currentDistance; ++baliseIter) {
            SendMessageThatBaliseWasPassed(**baliseIter);
            ++passedBalisesCnt;
        }
    } else {    // train is moving backwards
        //find the last balise which has lower position than previousDistance
        auto baliseIter = std::upper_bound(balises.rbegin(), balises.rend(), previousDistance,
                                           [](Distance distance, const std::unique_ptr<Balise>& balise) {
                                               return distance > balise->GetPos();
                                           });
        // send a message for every balise with position in the interval <currentDistance, previousDistance)
        for (; baliseIter != balises.rend() && (*baliseIter)->GetPos() >= currentDistance; ++baliseIter) {
            SendMessageThatBaliseWasPassed(**baliseIter);
            ++passedBalisesCnt;
        }
    }
    if (passedBalisesCnt) {
        printerService->PrintBalisesOnCurrentPosition(currentDistance, passedBalisesCnt);
    }
}

void BtmService::SendMessageThatBaliseWasPassed(const Balise& balise) {
    std::vector<std::shared_ptr<Packet>> packets = {std::make_shared<Packet>(EndOfInformation())};
    std::shared_ptr<BTMMessage> message = std::make_shared<BTMMessage>(
            balise.GetTelegram().Q_MEDIA,
            balise.GetTelegram().M_VERSION,
            balise.GetTelegram().Q_UPDOWN,
            balise.GetTelegram().N_TOTAL,
            balise.GetTelegram().M_MCOUNT,
            balise.GetTelegram().M_DUP,
            balise.GetTelegram().NID_C,
            balise.GetTelegram().N_PIG,
            balise.GetTelegram().Q_LINK,
            balise.GetTelegram().NID_BG,
            packets
    );
    
    jruLoggerService->Log(true, MessageType::Note, "BtmService: passed balise with id[%baliseId%] at [%distance%]m",
                          balise.GetBaliseId(), balise.GetPos().GetMeters());
    
    mqttPublisherService->Publish(message);
}
