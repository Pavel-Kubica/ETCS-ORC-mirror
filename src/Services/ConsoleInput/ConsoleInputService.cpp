/** @file ConsoleInputService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *
 *
 *  ### Contributors
 *  rehorja8
 */
 
#include <iostream>
#include <sstream>
#include "ConsoleInputService.hpp"

void ConsoleInputService::Initialize(ServiceContainer& container) {
    this->trainService = container.FetchService<ITrainControlUpdateService>().get();
    this->machineControlService = container.FetchService<IMachineControlDataService>().get();
    this->humanControlService = container.FetchService<IHumanControlDataService>().get();
    
}

void ConsoleInputService::Start(ServiceContainer& container) {
    this->workThread = std::thread(&ConsoleInputService::MainLoop, this);
}

void ConsoleInputService::Wait() {
    this->workThread.join();
}

void ConsoleInputService::AppExit() {

}

void ConsoleInputService::MainLoop() {
    // Za tento kód se stydím
    char input = 0;
    double argument;
    std::string line;
    
    auto invalidInput = [&]() { std::cerr << "[console-service]: invalid input" << line << std::endl; };
    
    while (true) {
        std::cout << "[console-service]: (enter input)>>> " << std::flush;
        
        line.clear();
        std::getline(std::cin, line);
        if (line.empty()) {
            invalidInput();
            continue;
        }
        std::istringstream stream(line);
        input = (char) stream.get();
        if (input == 'q') {
            return;
        }
        if (!(stream >> argument)) {
            invalidInput();
            continue;
        }
        
        switch (input) {
            case 'l':
                switch ((int) argument) {
                    case 1:
                        this->humanControlService->SetDrivingLever(DrivingLeverPosition::Accelerate);
                        break;
                    case 2:
                        this->humanControlService->SetDrivingLever(DrivingLeverPosition::Continue);
                        break;
                    case 3:
                        this->humanControlService->SetDrivingLever(DrivingLeverPosition::Neutral);
                        break;
                    case 4:
                        this->humanControlService->SetDrivingLever(DrivingLeverPosition::ElectrodynamicBrake);
                        break;
                    case 5:
                        this->humanControlService->SetDrivingLever(DrivingLeverPosition::PneumaticBrake);
                        break;
                    case 6:
                        this->humanControlService->SetDrivingLever(DrivingLeverPosition::QuickBrake);
                        break;
                    default:
                        invalidInput();
                        continue;
                }
                break;
            
            case 'd': {
                DirectionLeverPosition p;
                switch ((int) argument) {
                    case -1:
                        p = DirectionLeverPosition::Backwards;
                        break;
                    case 0:
                        p = DirectionLeverPosition::Neutral;
                        break;
                    case 1:
                        p = DirectionLeverPosition::Forwards;
                        break;
                    default:
                        invalidInput();
                        continue;
                }
                this->humanControlService->SetTrainDirection(p);
                break;
            }
            
            case 'b': {
                this->humanControlService->SetBattery((bool) argument);
                break;
            }
            
            case 'c': {
                bool cab = (bool)argument;
                this->humanControlService->SetCab(cab);
                break;
            }
            case 'p': {
                bool pantograph = (bool)argument;
                this->humanControlService->SetPantograph(pantograph);
                break;
            }
            case 'h': {
                bool horn = (bool)argument;
                this->humanControlService->SetHorn(horn);
                break;
            }
            case 'e': {
                auto engineBrake = (EngineBrakeLeverPosition)argument;
                this->humanControlService->SetEngineBrake(engineBrake);
                break;
            }
            case 'i': { // i0, i2, i4 are valid
                if (argument != 0 && argument != 2 && argument != 4) {
                    invalidInput();
                    continue;
                }
                auto light = (ForwardLight)argument;
                this->humanControlService->SetLight(light);
                break;
            }
            
            default:
                invalidInput();
                continue;
        }
        
        this->trainService->Update();
    }
}

