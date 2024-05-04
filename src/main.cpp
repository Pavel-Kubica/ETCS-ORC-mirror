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
    
    // these default values will be used only if the user does not enter different values as command line arguments
    std::string mqttHostname = "localhost";
    int mqttPort = 1883;

    parseArguments(argc, argv, mqttHostname, mqttPort);

    Application application;
    application.Initialize(mqttHostname, mqttPort);
    application.Run();
    
    return 0;
}
