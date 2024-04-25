/** @file ILpcManageable.hpp
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
 *  pavlian5
 */

#pragma once
/**
 * Interface for services that should respond to LPC commands
 */
class ILpcManageable {
public:
    explicit ILpcManageable() = default;
    virtual ~ILpcManageable() = default;
    /**
     * @retval true if successful
     * @retval false if not
     */
    virtual bool LpcSaidStart() = 0;

    /**
     * @retval true if successful
     * @retval false if not
     */
    virtual bool LpcSaidStop() = 0;

    /**
     * @retval true if successful
     * @retval false if not
     */
    virtual bool LpcSaidRestart() = 0;
};
