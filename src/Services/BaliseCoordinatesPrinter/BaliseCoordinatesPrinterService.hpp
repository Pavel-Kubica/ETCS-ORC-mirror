/** @file BaliseCoordinatesPrinterService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service responsible for printing balise coordinates to a file
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once

#include "IInitializable.hpp"
#include "IBaliseCoordinatesPrinterService.hpp"

class BaliseCoordinatesPrinterService: public IBaliseCoordinatesPrinterService, public IInitializable {

};
