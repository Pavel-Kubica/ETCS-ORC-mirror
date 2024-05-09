/** @file IBaliseDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for BaliseDataService
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once

#include "BaliseGroup.hpp"
#include "IService.hpp"

class IBaliseDataService : public IService {
public:
    virtual void SetBalises(std::vector<std::unique_ptr<Balise>> balises) = 0;

    virtual const std::vector<std::unique_ptr<Balise>>& GetBalisesSortedByDistance() const = 0;

    virtual void ClearData() = 0;

    static constexpr ServiceType Type = ServiceType::BaliseData;
};
