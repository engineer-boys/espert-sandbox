#ifndef ESPERT_SANDBOX_CURSORCOMPONENT_HH
#define ESPERT_SANDBOX_CURSORCOMPONENT_HH

#include "Espert.hh"
#include "MG1/Common/ObjectInfo.hh"
#include "MG1/Interfaces/Interfaces.hh"
#include "MG1/Utils.hh"

using namespace esp;

namespace mg1
{
  class CursorComponent : public IComponent, public IEventable
  {
   private:
    std::shared_ptr<CursorInfo> m_info;
    glm::vec3 m_previous_position;

   public:
    CursorComponent(uint32_t id, CursorType type, glm::vec3 position = { 0, 0, 0 });
    ~CursorComponent() = default;

    static std::tuple<std::vector<Vertex>, std::vector<uint32_t>> construct();

    inline CursorInfo* get_info() { return m_info.get(); }

    void update();
    void update_when_mouse_pressed();

    inline glm::vec3 get_position() { return m_node->get_translation(); }
    inline glm::vec3 get_delta_position() { return m_node->get_translation() - m_previous_position; }
    inline CursorType get_type() { return m_info->m_type; }
    inline bool is_type(CursorType type) { return m_info->m_type == type; }

    void handle_event(MouseMovedEvent& event, float dt, RotationAxis rotation_axis);
    void handle_event(MouseScrolledEvent& event);

   private:
    void update_mouse();
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORCOMPONENT_HH
