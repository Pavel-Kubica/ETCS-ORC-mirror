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

    [[nodiscard]] EngineBrakeLeverPosition GetEngineBrake() const override;

    void SetEngineBrake(EngineBrakeLeverPosition position) override;

    [[nodiscard]] bool GetQuickRelease() const override;

    void SetQuickRelease(bool on) override;

    [[nodiscard]] bool GetHorn() const override;

    void SetHorn(bool on) override;

    [[nodiscard]] bool GetSander() const override;

    void SetSander(bool on) override;

    [[nodiscard]] bool GetPantograph() const override;

    void SetPantograph(bool up) override;

    [[nodiscard]] bool GetEmergencyBrake() const override;

    void SetEmergencyBrake(bool on) override;

    [[nodiscard]] ForwardLight GetLight() const override;

    void SetLight(ForwardLight light) override;

    bool LightSkipped() const override;

private:
    std::atomic_bool battery;
    std::atomic_bool cab;
    std::atomic<EngineBrakeLeverPosition> engineBrake;
    std::atomic_bool quickRelease;
    std::atomic_bool horn;
    std::atomic_bool sander;
    std::atomic_bool pantograph;
    std::atomic_bool emergencyBrake;
    std::atomic<ForwardLight> forwardLight;
    std::atomic_bool lightSkipped;

private:
    std::atomic<DirectionLeverPosition> trainDirection;
    DrivingLeverPosition drivingLeverPosition;
    bool touchedRelease;

    // mutable is for const getters
    // - this example with mutable mutex is on cpp reference:
    // https://en.cppreference.com/w/cpp/language/cv
    mutable std::mutex leverPositionMutex{};
    mutable std::mutex lightMutex{};
};
