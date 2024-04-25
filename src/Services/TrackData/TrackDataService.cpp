/** @file TrackDataService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Class for storing all balisegroups, speed sections and gradients
 *
 *  ### Contributors
 *  veselo21
 *  pavlian5
 */

#include "TrackDataService.hpp"

void TrackDataService::Initialize(ServiceContainer& container) {

}
void TrackDataService::SetBaliseGroups(std::map<uint32_t, std::shared_ptr<BaliseGroup>> baliseGps) {
    baliseGroups = std::move(baliseGps);
}

std::vector<std::shared_ptr<BaliseGroup>> TrackDataService::GetBaliseGroupsSortedByDistance() const {
    std::vector<std::shared_ptr<BaliseGroup>> retVal;
    for(const auto &p : baliseGroups) {
        retVal.push_back(p.second);
    }
    std::sort(retVal.begin(), retVal.end(), []( const std::shared_ptr<BaliseGroup> lhs, const std::shared_ptr<BaliseGroup> rhs ) {
        return lhs->GetAbsPosition() < rhs->GetAbsPosition();
    });

    return retVal;
}

std::map<uint32_t, std::shared_ptr<BaliseGroup>>& TrackDataService::GetBaliseGroups() {
    return baliseGroups;
}

void TrackDataService::ClearData() {
    baliseGroups.clear();
}