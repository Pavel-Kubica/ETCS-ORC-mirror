/** @file ITrackDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for track data service
 *
 *  ### Contributors
 *  veselo21
 *  pavlian5
 */

#pragma once

#include "BaliseGroup.hpp"
#include "IService.hpp"

class ITrackDataService : public IService {
public:
    virtual void SetBaliseGroups(std::map<uint32_t, std::shared_ptr<BaliseGroup>> baliseGps) = 0;

    /**
     * @warning return value is a reference so handle with care
     */
    virtual std::map<uint32_t, std::shared_ptr<BaliseGroup>>& GetBaliseGroups() = 0;

    virtual std::vector<std::shared_ptr<BaliseGroup>> GetBaliseGroupsSortedByDistance() const = 0;

    virtual void ClearData() = 0;

    static constexpr ServiceType Type = ServiceType::TrackData;
};
