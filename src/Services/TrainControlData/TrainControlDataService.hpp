/** @file ITrainControlDataService.hpp
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

#include "ITrainControlDataService.hpp"

class TrainControlDataService : public ITrainControlDataService {
public:
    TrainControlDataService();

    [[nodiscard]] bool GetBattery() const override;

    void SetBattery(bool battery) override;

    [[nodiscard]] bool GetCab() const override;

    void SetCab(bool cab) override;

    [[nodiscard]] DirectionLeverPosition GetTrainDirection() const override;

    void SetTrainDirection(DirectionLeverPosition position) override;

    [[nodiscard]] DrivingLeverPosition GetDrivingLever() const override;

    void SetDrivingLever(DrivingLeverPosition position) override;

private:
    bool battery;
    bool cab;
    DirectionLeverPosition trainDirection;
    DrivingLeverPosition drivingLeverPosition;
};
