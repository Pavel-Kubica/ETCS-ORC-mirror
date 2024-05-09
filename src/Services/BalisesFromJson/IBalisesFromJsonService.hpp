/** @file IBalisesFromJsonService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for BalisesFromJsonService
 *
 *  ### Contributors
 *  pavlian5
 */


#pragma once


#include "IService.hpp"

class IBalisesFromJsonService: public IService {
public:
    static constexpr ServiceType Type = ServiceType::BalisesFromJson;
};
