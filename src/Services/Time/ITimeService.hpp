/** @file ITimeService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for time service
 *
 *  ### Contributors
 *  kaufmlu1
 *  dolezj41
 *  zimaluk1
 */

#pragma once

 #include "IService.hpp"
#include <cstdint>
/**
 * Interface for service responsible for managing current time
 */
class ITimeService : public IService {
public:
    ITimeService() : IService(){};
    virtual uint32_t GetCurrentTimestamp() const = 0;

    static constexpr ServiceType Type = ServiceType::Time;
};