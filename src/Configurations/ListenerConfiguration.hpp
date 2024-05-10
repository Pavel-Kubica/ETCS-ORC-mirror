/** @file ListenerConfiguration.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Listener Configuration
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk
 */

#pragma once


#include "Topic.hpp"
#include "json.hpp"

class ListenerConfiguration {
public:
    ListenerConfiguration() {
        // topics.push_back(Topic::LPCtoORC); this is here automatically see mqttListener initialize

        // TODO: check - this should be everything (?)
        topics.push_back(Topic::EVCtoODO);
        topics.push_back(Topic::EVCtoTIU);
        topics.push_back(Topic::CAN_1_0);
        topics.push_back(Topic::CAN_1_4);
        topics.push_back(Topic::CAN_1_5);

//        topics.push_back(Topic::RBCtoEVC);
//        topics.push_back(Topic::DMItoEVC);
//        topics.push_back(Topic::BTMtoEVC);
//        topics.push_back(Topic::ODOtoEVC);
//        topics.push_back(Topic::TIUtoEVC);
//        // topics.push_back(Topic::LPCtoEVC); this is here automatically see mqttListener initialize
//        topics.push_back(Topic::TEST);

    }
    static constexpr char FileName[] = "listener-configuration.json";

    std::vector<Topic> topics;

    void from_json(const nlohmann::json& j) {
        topics.clear();
        for (auto t : j.at("topics")) {
            topics.push_back(ConvertStringToTopic(t.get<std::string>()));
        }
    }

    nlohmann::json to_json() const {
        nlohmann::json j;
        j["topics"];
        for (auto x : topics) {
            j["topics"].push_back(ConvertTopicToString(x));
        }
        return j;
    }
};
