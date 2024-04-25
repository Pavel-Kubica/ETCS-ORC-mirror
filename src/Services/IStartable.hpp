/** @file IInitializable.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for all services which can be startable
 *
 *  ### Contributors
 *  kaufmlu1
 *  dolezj41
 *  zimaluk1
 */

#pragma once

#include "ServiceContainer.hpp"
/**
 * Interface for all services which can be startable
 */
class IStartable {
protected:
    friend class ServiceContainer;

    virtual void Start(ServiceContainer& container) = 0;
    virtual void Wait() = 0;
    virtual void AppExit() = 0;
};