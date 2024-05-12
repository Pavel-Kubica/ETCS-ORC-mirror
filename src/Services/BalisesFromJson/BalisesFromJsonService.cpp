/** @file BalisesFromJsonService.cpp
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


#include "BalisesFromJsonService.hpp"
#include <fstream>

void BalisesFromJsonService::Initialize(ServiceContainer& container) {
    baliseDataService = container.FetchService<IBaliseDataService>().get();
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    GetBalisesFromJson();   // check that the balise file is available and in the correct format
}

bool BalisesFromJsonService::LpcSaidStart() {
    baliseDataService->SetBalises(GetBalisesFromJson());
    return true;
}

bool BalisesFromJsonService::LpcSaidStop() {
    return true;
}

bool BalisesFromJsonService::LpcSaidRestart() {
    return LpcSaidStop() && LpcSaidStart();
}

std::vector<std::unique_ptr<Balise>> BalisesFromJsonService::GetBalisesFromJson() const {
    const std::string fileName = "balises.json";
    auto path = std::filesystem::current_path() / "configurations";
    bool fileWasFound = false;
    for (int i = 0; i < 5; i++) {
        if (std::filesystem::exists(path / fileName)) {
            fileWasFound = true;
            break;
        }
        path = path.parent_path();
    }
    if (!fileWasFound) {
        jruLoggerService->Log(true, MessageType::Error, "BalisesFromJsonService: Could not find file " + fileName);
        return {};
    }

    auto pathToFile = path / fileName;
    std::ifstream baliseFile(pathToFile.string());
    if (!baliseFile) {
        jruLoggerService->Log(true, MessageType::Error,
                              "BalisesFromJsonService: Found file " + pathToFile.string() + " but could not open it.");
        return {};
    }
    nlohmann::json baliseJson;
    try {
        baliseJson = nlohmann::json::parse(baliseFile);
    } catch (const nlohmann::json::exception& e) {
        jruLoggerService->Log(true, MessageType::Error,
                              "BalisesFromJsonService: Error while parsing balise file: " + std::string(e.what()));
        return {};
    }
    std::vector<std::unique_ptr<Balise>> balises;
    try {
        for (auto& b: baliseJson) {
            balises.push_back(BaliseFromJson(b));
        }
    } catch (const nlohmann::json::exception& e) {
        jruLoggerService->Log(true, MessageType::Error,
                              "BalisesFromJsonService: Json in file " + pathToFile.string() + " does not contain balises in the correct format: " + e.what());
        return {};
    }
    jruLoggerService->Log(true, MessageType::Note,"BalisesFromJsonService: Balises ware correctly parsed from file " + pathToFile.string());
    return balises;
}

std::unique_ptr<Balise> BalisesFromJsonService::BaliseFromJson(const nlohmann::json& baliseJson) {
    Telegram telegram{};
    const nlohmann::json& telegramJson = baliseJson.at("telegram");
    telegram.Q_UPDOWN = telegramJson.at("Q_UPDOWN");
    telegram.M_VERSION = telegramJson.at("M_VERSION");
    telegram.Q_MEDIA = telegramJson.at("Q_MEDIA");
    telegram.N_PIG = telegramJson.at("N_PIG");
    telegram.N_TOTAL = telegramJson.at("N_TOTAL");
    telegram.M_DUP = telegramJson.at("M_DUP");
    telegram.M_MCOUNT = telegramJson.at("M_MCOUNT");
    telegram.NID_C = telegramJson.at("NID_C");
    telegram.NID_BG = telegramJson.at("NID_BG");
    telegram.Q_LINK = telegramJson.at("Q_LINK");

    Distance distance;
    distance.SetDistanceFromMeters(baliseJson.at("balisePosInMeters"));

    return std::make_unique<Balise>(
            baliseJson.at("baliseId"),
            telegram,
            distance
    );
}
