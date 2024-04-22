/** @file ILpcManagable.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.6.0
 *
 *  ### Description
 *  Interface for services that should respond to LPC commands
 *
 *  ### Contributors
 *  veselo21
 *  maresj39
 */

#pragma once
/**
 * Interface for services that should respond to LPC commands
 */
class ILpcManagable {
public:
    explicit ILpcManagable() = default;
    virtual ~ILpcManagable() = default;
    virtual void LpcSaidStart() = 0;
    virtual void LpcSaidStop() = 0;
    virtual void LpcSaidRestart() = 0;
};
