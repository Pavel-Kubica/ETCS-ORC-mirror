/** @file BaliseDataService.hpp
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

#pragma once

#include <memory>
#include <vector>
#include "IInitializable.hpp"
#include "IBaliseDataService.hpp"

class BaliseDataService : public IBaliseDataService {
public:
    void SetBalises(std::vector<std::unique_ptr<Balise>> balises) override;

    [[nodiscard]] const std::vector<std::unique_ptr<Balise>>& GetBalisesSortedByDistance() const override;

    /**
     * Clears all data stored in this service
     */
    void ClearData() override;

private:
    std::vector<std::unique_ptr<Balise>> balisesSortedByDistance;
};
