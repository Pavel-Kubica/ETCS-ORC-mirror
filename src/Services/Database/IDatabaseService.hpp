/** @file IDatabaseService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for Database Service
 *
 *  ### Contributors
 *  veselo21
 */

#pragma once

#include <string>

 #include "IService.hpp"


class IDatabaseService : public IService {
public:
    virtual void SetCredentials(const std::string& username,
                                const std::string& password,
                                const std::string& schema,
                                const std::string& address,
                                uint32_t timeout) = 0;

    virtual uint32_t GetTimeout() const = 0;

    static constexpr ServiceType Type = ServiceType::Database;
};