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
 */

#include "IHumanControlDataService.hpp"
#include <atomic>
#include "AsyncProperty.hpp"

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

private:
    std::atomic_bool battery;
    std::atomic_bool cab;
    AsyncProperty<DirectionLeverPosition> trainDirection;
    AsyncProperty<DrivingLeverPosition> drivingLeverPosition;
};
