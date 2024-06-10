/** @file IConsoleInputService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Class for manual input of controls (instead of physical panel).
 *  Used mainly for debugging and testing.
 *
 *  ### Contributors
 *  rehorja8
 */
 
#pragma once


#include "ServiceType.hpp"
#include "IService.hpp"

class IConsoleInputService : public IService {
public:
    static constexpr ServiceType Type = ServiceType::ConsoleInput;
};
