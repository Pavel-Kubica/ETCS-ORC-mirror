/** @file BaliseCoordinatesPrinterConfiguration.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Configuration of BaliseCoordinatesPrinterService
 *
 *  ### Contributors
 *  pavlian5
 */

#include "BaliseCoordinatesPrinterConfiguration.hpp"

constexpr char FILENAME[] = "balise-coordinates-filename";
constexpr char SHOULD_PRINT_BALISES[] = "should-print-balises";

void BaliseCoordinatesPrinterConfiguration::from_json(const nlohmann::json& j) {
    this->baliseCoordinatesFilename = j[FILENAME];
    this->shouldPrintBalises = j[SHOULD_PRINT_BALISES];
}

nlohmann::json BaliseCoordinatesPrinterConfiguration::to_json() const {
    nlohmann::json j;

    j[FILENAME] = this->baliseCoordinatesFilename;
    j[SHOULD_PRINT_BALISES] = this->shouldPrintBalises;

    return j;
}

