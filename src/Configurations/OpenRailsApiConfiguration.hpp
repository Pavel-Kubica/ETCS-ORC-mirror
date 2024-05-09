/** @file OpenRailsApiConfiguration.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Configuration of OpenRails' URL API endpoint.
 *  See src/Services/Configuration/ConfigurationService for more info of how this works.
 *
 *  ### Contributors
 *  rehorja8
 */

#pragma once


#include "json.hpp"

class OpenRailsApiConfiguration {
public:
    static constexpr char FileName[] = "open-rails-api-configuration.json";
    
    void from_json(const nlohmann::json& j);
    [[nodiscard]] nlohmann::json to_json() const;
    
    std::string cabControlsUrl = "http://127.0.0.1:2150/API/CABCONTROLS";
    std::string orcUrl = "http://127.0.0.1:2150/API/ORC";
    std::chrono::milliseconds cabControlsTimeout{1000};
    std::chrono::milliseconds orcTimeout{1000};
    std::chrono::milliseconds apiGetOrcCallingInterval{600};
};
