
#include "Message.hpp"


class OrcToGuiMessage : Message {
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
