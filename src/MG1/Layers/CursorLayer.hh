#ifndef ESPERT_SANDBOX_CURSORLAYER_HH
#define ESPERT_SANDBOX_CURSORLAYER_HH

#include "Espert.hh"
#include "MG1/Events/Gui/GuiEvents.hh"

using namespace esp;

namespace mg1
{
  class CursorLayer : public Layer
  {
   private:
    std::shared_ptr<EspShader> m_shader;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    std::unique_ptr<EspIndexBuffer> m_index_buffer;

    bool m_update{ true };

   public:
    CursorLayer();

    virtual void update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

    bool gui_mouse_state_changed_event_handler(GuiMouseStateChangedEvent& event);
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORLAYER_HH
