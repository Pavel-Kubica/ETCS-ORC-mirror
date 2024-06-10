/** @file HumanControlDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Data service storing current train control data
 *
 *  ###Contributors
 *  kubicpa3
 *  rehorja8
 */

#include <atomic>
#include "AsyncProperty.hpp"
#include "IHumanControlDataService.hpp"

class HumanControlDataService : public IHumanControlDataService {
public:
    HumanControlDataService();

    [[nodiscard]] bool GetBattery() const override;

    void SetBattery(bool battery) override;

    [[nodiscard]] bool GetCab() const override;

    void SetCab(bool cab) override;

    [[nodiscard]] DirectionLeverPosition GetTrainDirection() const override;

    void SetTrainDirection(DirectionLeverPosition position) override;

    [[nodiscard]] DrivingLeverPosition GetDrivingLever() const override;

    void SetDrivingLever(DrivingLeverPosition position) override;
    
    [[nodiscard]] bool HasTouchedRelease() const override;
    
private:
    std::atomic_bool battery;
    std::atomic_bool cab;

private:
    std::atomic<DirectionLeverPosition> trainDirection;
    DrivingLeverPosition drivingLeverPosition;
    bool touchedRelease;

    // mutable is for const getters
    // - this example with mutable mutex is on cpp reference:
    // https://en.cppreference.com/w/cpp/language/cv
    mutable std::mutex leverPositionMutex{};
};
