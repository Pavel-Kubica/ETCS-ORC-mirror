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

    [[nodiscard]] bool getBattery() const override;

    void setBattery(bool battery) override;

    [[nodiscard]] bool getCab() const override;

    void setCab(bool cab) override;

    [[nodiscard]] DirectionLeverPosition getTrainDirection() const override;

    void setTrainDirection(DirectionLeverPosition position) override;

    [[nodiscard]] DrivingLeverPosition getDrivingLever() const override;

    void setDrivingLever(DrivingLeverPosition position) override;

private:
    bool battery;
    bool cab;
    DirectionLeverPosition trainDirection;
    DrivingLeverPosition drivingLeverPosition;
};
