#ifndef ESPERT_SANDBOX_TORUSCOMPONENT_HH
#define ESPERT_SANDBOX_TORUSCOMPONENT_HH

#include "Espert.hh"
#include "MG1/Common/ObjectInfo.hh"
#include "MG1/Common/TorusInit.hh"
#include "MG1/Events/Gui/GuiEvents.hh"
#include "MG1/Interfaces/Interfaces.hh"

using namespace esp;

namespace mg1
{
  class TorusComponent : IEventable
  {
   private:
    uint32_t m_id;
    std::string m_name;

    std::shared_ptr<Node> m_node;

    std::shared_ptr<TorusInfo> m_info;

   public:
    TorusComponent(uint32_t id,
                   float R           = TorusInit::S_R,
                   float r           = TorusInit::S_r,
                   int density_theta = TorusInit::S_DENSITY_THETA,
                   int density_phi   = TorusInit::S_DENSITY_PHI);
    ~TorusComponent() = default;

    std::tuple<std::vector<Vertex>, std::vector<uint32_t>> reconstruct();

    inline std::shared_ptr<Node> get_node() { return m_node; }
    inline TorusInfo* get_info() { return m_info.get(); }

    void handle_event(MouseMovedEvent& event, float dt, int rotation_axis);
    void handle_event(MouseScrolledEvent& event);
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_TORUSCOMPONENT_HH
