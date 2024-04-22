/** @file IInitializable.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for all services
 *
 *  ### Contributors
 *  kaufmlu1
 *  dolezj41
 *  zimaluk1
 */

#pragma once

 #include "ServiceType.hpp"
/**
 * Interface for classes that act as a Service
 */
class IService {
public:
    explicit IService() = default;
    virtual ~IService() = default;

    static constexpr ServiceType Type() { return ServiceType::None; }
};
