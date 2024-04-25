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
    trackDataService = container.FetchService<ITrackDataService>().get();
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
    if (!LoadBaliseGroups()) {
        jruLoggerService->Log(MessageType::Fatal, "Could not load balises from database");
        return false;
    }
    return true;
}

bool DatabaseService::LoadBaliseGroups() {
    try {
        sqlStatement = sqlConnection->createStatement();
        sqlResultSet = sqlStatement->executeQuery(loadBaliseInfo);
    } catch (sql::SQLException& e) {
        jruLoggerService->Log(true, MessageType::Error, e.what());
        delete sqlStatement;
        delete sqlResultSet;
        return false;
    }

    std::map<uint32_t, std::shared_ptr<BaliseGroup>> baliseGroups;

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
            delete sqlStatement;
            delete sqlResultSet;
            return false;
        }

        Balise balise(baliseID, telegram, absPos);

        bool isLast = (telegram.N_PIG == telegram.N_TOTAL);  // this determines if this balise is last in its group

        if (baliseGroups.find(telegram.NID_BG) == baliseGroups.end()) {  // if bgID is not known yet, create new group
            baliseGroups.insert({telegram.NID_BG, std::make_shared<BaliseGroup>(BaliseGroup(telegram.NID_BG))});
        }
        baliseGroups[telegram.NID_BG]->GetBalises().push_back(std::make_shared<Balise>(balise));  // add this balise to its balise group

        if (isLast) {  // if this balise is last then we base this balisgroup position on this balise
            baliseGroups[telegram.NID_BG]->SetAbsPosition(absPos);
        }
    }

    trackDataService->SetBaliseGroups(baliseGroups);

    jruLoggerService->Log(true, MessageType::Info, "Successfully loaded balise groups from database");

    delete sqlStatement;
    delete sqlResultSet;
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
    sqlDriver = get_driver_instance();
    try {
        sqlConnection = sqlDriver->connect(databaseAddress, databaseUsername, databasePassword);
    } catch (sql::SQLException& e) {
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
    delete sqlConnection;

    return true;
}

bool DatabaseService::LpcSaidStart() {
    if(connected) return false;

    if (defaultCredentials) {  // using default database setting if non were sent
        jruLoggerService->Log(true, MessageType::Warning, "Rbc did not receive any db connection credentials, using default ones");
    }
    if(!Connect()) return false;
    if(!LoadDataFromDatabase()) return false;
    return true;
}

bool DatabaseService::LpcSaidStop() {
    if(!connected) return false;

    if(!Disconnect()) return false;
    SetDefaultCredentials();
    trackDataService->ClearData();
    return true;
}

bool DatabaseService::LpcSaidRestart() {
    if(!connected) return false;

    trackDataService->ClearData();
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
