/** @file BalisesFromJsonService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service responsible for loading balises from json
 *
 *  ### Contributors
 *  pavlian5
 */


#pragma once

#include "IBalisesFromJsonService.hpp"
#include "ILpcManageable.hpp"
#include "IInitializable.hpp"
#include "IBaliseDataService.hpp"
#include "JRULoggerService.hpp"

class BalisesFromJsonService: public IBalisesFromJsonService, public IInitializable, public ILpcManageable {
public:
    bool LpcSaidStart() override;

    bool LpcSaidStop() override;

    bool LpcSaidRestart() override;

protected:
    void Initialize(ServiceContainer& container) override;

private:
    std::vector<std::unique_ptr<Balise>> GetBalisesFromJson() const;

    static std::unique_ptr<Balise> BaliseFromJson(const nlohmann::json& baliseJson);

    IBaliseDataService* baliseDataService;
    JRULoggerService* jruLoggerService;
};
