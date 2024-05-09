/** @file DatabaseService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *   Service responsible for connecting and communicating with mySql database
 *
 *  ### Contributors
 *  veselo21
 *  pavlian5
 */

#include "DatabaseService.hpp"
#include <queue>
#include <set>
#include "DatabaseQueries.hpp"
#ifdef WINDOWS
#include <direct.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

void DatabaseService::Initialize(ServiceContainer& container) {
    baliseDataService = container.FetchService<IBaliseDataService>().get();
    jruLoggerService = container.FetchService<JRULoggerService>().get();

    SetDefaultCredentials();
}

bool DatabaseService::Connect() {
    uint16_t timeout = 0;

    int secsSleep = 1;
    while (!ConnectToDatabase()) {
        timeout++;
        #ifdef WINDOWS
            Sleep(secsSleep * 1000);
        #else
            sleep(secsSleep);
        #endif
        jruLoggerService->Log(true, MessageType::Warning, "Could not connect to a database (Try#%timeout%)", timeout);
        if (timeout >= databaseTimeout) {
            jruLoggerService->Log(true, MessageType::Fatal, "Could not connect to a database");
            SetDefaultCredentials();
            return false;
        }
        std::cerr << "Retrying..." << std::endl;
    }
    return true;
}

bool DatabaseService::LoadDataFromDatabase() {
    if (!LoadBalises()) {
        jruLoggerService->Log(MessageType::Fatal, "Could not load balises from database");
        return false;
    }
    return true;
}

bool DatabaseService::LoadBalises() {
    std::unique_ptr<sql::Statement> sqlStatement;
    std::unique_ptr<sql::ResultSet> sqlResultSet;
    try {
        sqlStatement = std::unique_ptr<sql::Statement>(sqlConnection->createStatement());
        sqlResultSet = std::unique_ptr<sql::ResultSet>(sqlStatement->executeQuery(loadBaliseInfo));
    } catch (const sql::SQLException& e) {
        jruLoggerService->Log(true, MessageType::Error, e.what());
        return false;
    }

    std::vector<std::unique_ptr<Balise>> balises;

    while (sqlResultSet->next()) {
        // Getting the data from this result row
        std::string baliseID = sqlResultSet->getString(1);
        std::string telegramString = sqlResultSet->getString(2);
        std::string pos = sqlResultSet->getString(3);

        // Parsing data from the results
        Telegram telegram(telegramString);
        Distance absPos;
        try {
            absPos.SetDistanceFromMeters(std::stol(pos));
        } catch (std::exception& e) {
            jruLoggerService->Log(true, MessageType::Error, e.what());
            return false;
        }

        Balise balise(baliseID, telegram, absPos);

        balises.push_back(std::make_unique<Balise>(balise));
    }

    baliseDataService->SetBalises(std::move(balises));

    jruLoggerService->Log(true, MessageType::Info, "Successfully loaded balises from database");

    return true;
}

void DatabaseService::SetCredentials(const std::string& username,
                                     const std::string& password,
                                     const std::string& schema,
                                     const std::string& address,
                                     uint32_t timeout) {
    databaseAddress = address;
    databaseSchema = schema;
    databaseUsername = username;
    databasePassword = password;
    databaseTimeout = timeout;  // in seconds
    defaultCredentials = false;
}
uint32_t DatabaseService::GetTimeout() const {
    return databaseTimeout;
}

bool DatabaseService::ConnectToDatabase() {
    // cannot be smart ptr because destructor is protected
    // the library takes care of freeing the driver object
    sql::Driver* sqlDriver = get_driver_instance();
    try {
        sqlConnection = std::unique_ptr<sql::Connection>(sqlDriver->connect(databaseAddress, databaseUsername, databasePassword));
    } catch (const sql::SQLException& e) {
        jruLoggerService->Log(true, MessageType::Error, e.what());
        return false;
    }
    sqlConnection->setSchema(databaseSchema);
    jruLoggerService->Log(true, MessageType::Info, "Successfully connected to database");
    connected = true;

    return true;
}

bool DatabaseService::Disconnect() {
    if(!connected) return false;

    connected = false;

    return true;
}

bool DatabaseService::LpcSaidStart() {
    if(connected) return false;

    if (defaultCredentials) {  // using default database setting if non were sent
        jruLoggerService->Log(true, MessageType::Warning, "ORC did not receive any db connection credentials, using default ones");
    }
    if(!Connect()) return false;
    if(!LoadDataFromDatabase()) return false;
    return true;
}

bool DatabaseService::LpcSaidStop() {
    if(!connected) return false;

    if(!Disconnect()) return false;
    SetDefaultCredentials();
    baliseDataService->ClearData();
    return true;
}

bool DatabaseService::LpcSaidRestart() {
    if(!connected) return false;

    baliseDataService->ClearData();
    if(!LoadDataFromDatabase()) return false;
    return true;
}

void DatabaseService::SetDefaultCredentials() {
    // default values
    databaseAddress = "localhost";
    databaseSchema = "infradb";
    databaseUsername = "root";
    databasePassword = "root";
    databaseTimeout = 10;  // in seconds
    defaultCredentials = true;
}
