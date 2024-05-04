/** @file main.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Main of the whole program
 *
 *  ### Contributors
 *  kaufmlu1
 *  spakoro1
 */
#include <string>
#include <iostream>
#include <cpr/cpr.h>
#include "Application.hpp"
void parseArguments(int argc, char** argv, std::string& mqttHostname, int& mqttPort) {
    if (argc > 1) {
        mqttHostname = argv[1];
    }
    if (argc > 2) {
        try {
            auto arg2 = std::stoi(argv[2]);
            // port cannot be negative number
            if (arg2 > -1)
                mqttPort = arg2;
        } catch (const std::exception& e) {
        }
    }
}

int main(int argc, char** argv) {
    
    // TODO - remove: this is for a test
    std::cout << "Testing cpr library, type enter..." << std::endl;
    getchar();
    
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                               cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                               cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    std::cout << "status code: " << r.status_code << "\n";                  // 200
    std::cout << "content type:" << r.header["content-type"] << "\n";       // application/json; charset=utf-8
    std::cout << "content:\n " << r.text << "\n";                         // JSON text string
    std::cout << "waiting for enter..." << std::endl;
    getchar();
    
    // TODO: un-comment this
//    // these default values will be used only if the user does not enter different values as command line arguments
//    std::string mqttHostname = "localhost";
//    int mqttPort = 1883;
//
//    parseArguments(argc, argv, mqttHostname, mqttPort);
//
//    Application application;
//    application.Initialize(mqttHostname, mqttPort);
//    application.Run();
    
    return 0;
}
