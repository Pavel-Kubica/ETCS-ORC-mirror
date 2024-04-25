/** @file IInitializable.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for all services which can be initialized
 *
 *  ### Contributors
 *  kaufmlu1
 *  dolezj41
 *  zimaluk1
 */

#pragma once

#include "ServiceContainer.hpp"
/**
 * Interface for all services which can be initialized
 */
class IInitializable {
public:
    IInitializable() = default;

protected:
    friend class ServiceContainer;
    /**
     * Initialization of a service, usually from configuration files
     */
    virtual void Initialize(ServiceContainer& container) = 0;
};
