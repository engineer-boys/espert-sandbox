#ifndef ESPERT_SANDBOX_POINTCOMPONENT_HH
#define ESPERT_SANDBOX_POINTCOMPONENT_HH

#include "Espert.hh"
#include "MG1/Common/InitInfo.hh"
#include "MG1/Common/ObjectInfo.hh"
#include "MG1/Events/Gui/GuiEvents.hh"
#include "MG1/Events/Object/ObjectEvents.hh"
#include "MG1/Interfaces/Interfaces.hh"
#include "MG1/Utils.hh"

using namespace esp;

namespace mg1
{
  class PointComponent : public IComponent, public IEventable
  {
   private:
    std::shared_ptr<PointInfo> m_info;

   public:
    PointComponent(uint32_t id, float r = PointInit::S_R);
    ~PointComponent() = default;

    std::tuple<std::vector<Vertex>, std::vector<uint32_t>> reconstruct();
    void check_if_clicked();

    inline PointInfo* get_info() { return m_info.get(); }

    void handle_event(CursorPosChangedEvent& event);
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_POINTCOMPONENT_HH
