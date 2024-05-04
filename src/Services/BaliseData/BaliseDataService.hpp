/** @file BaliseDataService.hpp
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

#pragma once

#include <memory>
#include <vector>
#include "IInitializable.hpp"
#include "IBaliseDataService.hpp"

class BaliseDataService : public IBaliseDataService, public IInitializable {
public:
    void Initialize(ServiceContainer& container) override;

    /**
     * Saves balise groups loaded from database for later use by other services
     * @param baliseGps to be saved
     */
    void SetBaliseGroups(std::map<uint32_t, std::shared_ptr<BaliseGroup>> baliseGps) override;

    /**
     * @warning return value is a reference so handle with care
     */
    std::map<uint32_t, std::shared_ptr<BaliseGroup>>& GetBaliseGroups() override;

    /**
     * Automatically copies and sorts balise groups conveniently by geographical distance
     * @returns sorted balises in ascending order
     */
    [[nodiscard]] std::vector<std::shared_ptr<BaliseGroup>> GetBaliseGroupsSortedByDistance() const override;

    /**
     * Clears all data stored in this service
     */
    void ClearData() override;

private:
    std::map<uint32_t, std::shared_ptr<BaliseGroup>> baliseGroups;
};