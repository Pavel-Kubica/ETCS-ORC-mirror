/** @file DatabaseService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service responsible for connecting and communicating with mySql database
 *
 *  ### Contributors
 *  veselo21
 *  pavlian5
 */

#pragma once

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <memory>
#include <string>
#include "ServiceContainer.hpp"
#include "ConfigurationService.hpp"
#include "IInitializable.hpp"
#include "JRULoggerService.hpp"
#include "IBaliseDataService.hpp"
#include "IDatabaseService.hpp"

class DatabaseService : public IDatabaseService, public IInitializable, public ILpcManageable {
public:
    void Initialize(ServiceContainer& container) override;

    /**
     * Connects to a database and loads data into baliseDataService
     */
    bool LpcSaidStart() override;

    /**
     *  Disconnects from a database and clears data from baliseDataService
     */
    bool LpcSaidStop() override;

    /**
     * Clears data from baliseDataService and loads in fresh ones
     */
    bool LpcSaidRestart() override;

    /**
     * Sets the login credentials to be different from the default ones
     *
     * @param timeout number of retries (one try each second)
     *
     */
    void SetCredentials(const std::string& username,
                        const std::string& password,
                        const std::string& schema,
                        const std::string& address,
                        uint32_t timeout) override;

    [[nodiscard]] uint32_t GetTimeout() const override;

private:
    bool Connect();

    bool Disconnect();

    void SetDefaultCredentials();

    bool ConnectToDatabase();

    /**
     * Calls sql scripts from DatabaseQueries.hpp and feeds data into baliseDataService
     * @retval true if success
     * @retval false if not
     */
    bool LoadDataFromDatabase();

    bool LoadBaliseGroups();

    bool connected = false;
    bool defaultCredentials = true;

    IBaliseDataService* baliseDataService;
    JRULoggerService* jruLoggerService;

    // Database credentials
    std::string databaseAddress;
    std::string databaseSchema;
    std::string databaseUsername;
    std::string databasePassword;
    uint32_t databaseTimeout;

    // sql library shenanigans
    std::unique_ptr<sql::Connection> sqlConnection;
};
