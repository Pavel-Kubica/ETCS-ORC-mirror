/** @file ConfigurationService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service responsible for all configurations in the component
 *
 *  ### Contributors
 *  spankro1
 *  zimaluk1
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include "ServiceType.hpp"
#include "IService.hpp"
#include "json.hpp"
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
/**
 * Service which generates configuration files and fetches them for other services to get configured by
 */
class ConfigurationService : public IService {
public:
    static constexpr ServiceType Type = ServiceType::Configuration;
    /**
     * Reads the configuration file for T and constructs/returns a configuration class for T
     * @tparam T The target Service type
     * @return Configuration Class for the T service type
     */
    template <class T>
    T GetConfiguration() const{
        T configuration;
        std::string file_name = config_location + T::FileName;
        std::ifstream ifs(file_name);
        if (!ifs) {
            throw std::runtime_error("Could not open configuration file");
        }
        nlohmann::json file = nlohmann::json::parse(ifs);
        configuration.from_json(file);

        return configuration;
    };
    /**
     * Checks whether a configuration file for T exists
     * @tparam T The target Service type
     * @return If a configuration file for T exists
     */
    template <class T>
    bool ConfigurationExists() {
        std::filesystem::path config_file{config_location + T::FileName};
        return std::filesystem::exists(config_file);
    };
    /**
     * Creates a configuration file from a configuration class for T
     * @tparam T The target Service type
     */
    template <class T>
    void CreateConfiguration() {
        nlohmann::json configuration_file;
        T configuration;
        std::string file_name = config_location + T::FileName;
        configuration_file = configuration.to_json();

        std::ofstream out(file_name);
        out << configuration_file.dump(4);
        out.close();
    };
    /**
     * Combines all the other methods. Creates a configuration file if it doesnt exist,
     * and then constructs a conf. class from that file and returns it for T
     * @tparam T The target Service type
     * @return Configuration class for the T service type
     */
    template <class T>
    T FetchConfiguration() {
        std::string currentPath = std::filesystem::current_path().string();
        if (currentPath.empty()) {
            throw std::runtime_error("Could not fetch config location");
        }

        config_location = currentPath;
        config_location += "/configurations";
        config_location += "/";
        bool exists = ConfigurationExists<T>();
        if (!exists)
            CreateConfiguration<T>();
        return GetConfiguration<T>();
    }
    std::string config_location = "";
};
