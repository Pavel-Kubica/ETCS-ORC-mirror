/** @file ITimeService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Time service
 *
 *  ### Contributors
 *  kaufmlu1
 *  dolezj41
 *  zimaluk1
 */

#pragma once

#include "ITimeService.hpp"
#include <chrono>
/**
 * Service responsible for managing current time
 */
class TimeService : public ITimeService {
public:
    /**
     * @return Current timestamp in milliseconds
     */
    uint32_t GetCurrentTimestamp() const override;

private:
    std::chrono::high_resolution_clock Clock;
};
