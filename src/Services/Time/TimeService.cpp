/** @file ITimeService.cpp
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

#include "TimeService.hpp"

uint32_t TimeService::GetCurrentTimestamp() const{
    return std::chrono::duration_cast<std::chrono::milliseconds>(Clock.now().time_since_epoch()).count();
}
