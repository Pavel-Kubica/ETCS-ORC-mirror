/** @file BaliseCoordinatesPrinterConfiguration.hpp
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

#pragma once


#include "json.hpp"

class BaliseCoordinatesPrinterConfiguration {
public:
    static constexpr char FileName[] = "balise-coordinates-printer-config.json";

    void from_json(const nlohmann::json& j);

    [[nodiscard]] nlohmann::json to_json() const;

    std::string baliseCoordinatesFilename;
};
