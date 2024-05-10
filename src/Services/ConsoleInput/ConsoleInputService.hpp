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
 
#pragma once

#include <thread>
#include "IConsoleInputService.hpp"
#include "IInitializable.hpp"
#include "IStartable.hpp"
#include "ITrainControlUpdateService.hpp"
#include "IMachineControlDataService.hpp"
#include "IHumanControlDataService.hpp"

class ConsoleInputService : public IConsoleInputService,
                            public IInitializable,
                            public IStartable {
protected:
    void Initialize(ServiceContainer& container) override;
    void Start(ServiceContainer& container) override;
    void Wait() override;
    void AppExit() override;
    
private:
    void MainLoop();
    std::thread workThread;
    
    ITrainControlUpdateService* trainService;
    IMachineControlDataService* machineControlService;
    IHumanControlDataService* humanControlService;
    
};
