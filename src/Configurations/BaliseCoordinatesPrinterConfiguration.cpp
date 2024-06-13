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

void BaliseCoordinatesPrinterConfiguration::from_json(const nlohmann::json& j) {
    this->baliseCoordinatesFilename = j[FILENAME];
}

nlohmann::json BaliseCoordinatesPrinterConfiguration::to_json() const {
    nlohmann::json j;

    j[FILENAME] = this->baliseCoordinatesFilename;

    return j;
}

