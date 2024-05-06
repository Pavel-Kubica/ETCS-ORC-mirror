/** @file IGuiSimulationStateSenderService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for Gui sender service
 *
 *  ###Contributors
 *  nguyem12
 */

#include "ISimulationStateSender.hpp"
#include "IService.hpp"

class IGuiSimulationStateSenderService : public ISimulationStateSender,
                                         public IService {
public:
    static constexpr ServiceType Type = ServiceType::GuiSimulationStateSender;
};