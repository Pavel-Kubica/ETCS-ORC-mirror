/** @file BaliseDataService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Class for storing balises
 *
 *  ### Contributors
 *  pavlian5
 */

#include "BaliseDataService.hpp"


void BaliseDataService::SetBalises(std::vector<std::unique_ptr<Balise>> balises) {
    balisesSortedByDistance = std::move(balises);
    std::sort(balisesSortedByDistance.begin(), balisesSortedByDistance.end(),
              [](const std::unique_ptr<Balise>& lhs, const std::unique_ptr<Balise>& rhs) {
                  return lhs->GetPos() < rhs->GetPos();
              });
}

const std::vector<std::unique_ptr<Balise>>& BaliseDataService::GetBalisesSortedByDistance() const {
    return balisesSortedByDistance;
}

void BaliseDataService::ClearData() {
    balisesSortedByDistance.clear();
}
