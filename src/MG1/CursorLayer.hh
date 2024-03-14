#ifndef ESPERT_SANDBOX_CURSORLAYER_HH
#define ESPERT_SANDBOX_CURSORLAYER_HH

#include "Espert.hh"
#include "Gui/GuiEvents/GuiEvents.hh"

using namespace esp;

namespace mg1
{
  class CursorLayer : public Layer
  {
   private:
    struct CursorParams
    {
      alignas(16) float m_cursor_size;
      alignas(16) glm::vec3 m_cursor_color;
      float m_aspect_ratio;
    };

    std::shared_ptr<EspShader> m_shader;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    bool m_update{ true };

   public:
    CursorLayer();

    virtual void update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

    bool gui_mouse_state_changed_event_handler(GuiMouseStateChangedEvent& event);
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORLAYER_HH
