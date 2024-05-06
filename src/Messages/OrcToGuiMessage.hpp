/** @file OrcToGuiMessage.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Orc data message to Gui applications
 *
 *  ###Contributors
 *  nguyem12
 */

#include "Message.hpp"


class OrcToGuiMessage : public Message {
public:
    explicit OrcToGuiMessage();

    OrcToGuiMessage(double speedMpS,
                    double motiveForceN,
                    double brakeCylinderPSI,
                    double leadPipePSI);

    [[nodiscard]] nlohmann::json to_json() const override;
    void from_json(const nlohmann::json& j) override;

protected:
    double speedMpS;
    double motiveForceN;
    double brakeCylinderPSI;
    double leadPipePSI;
};
