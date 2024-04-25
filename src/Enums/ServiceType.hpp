/** @file ServiceType.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Enum of service types
 *
 *  ### Contributors
 *  spakoro1
 *  kaufmlu1
 *  zimaluk1
 */

#pragma once
/**
 * Enum of service types
 */
enum class ServiceType {
    None,
    Listener,
    Publisher,
    Configuration,
    MessageHandlers,
    Logger,
    Time,
    Database,
    TrackData
};
