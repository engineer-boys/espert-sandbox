#ifndef ESPERT_SANDBOX_CURSORLAYER_HH
#define ESPERT_SANDBOX_CURSORLAYER_HH

#include "Espert.hh"
#include "MG1/Components/CursorComponent.hh"
#include "MG1/Events/Gui/GuiEvents.hh"

using namespace esp;

namespace mg1
{
  class CursorLayer : public Layer
  {
   private:
    bool m_update{ true };

    Scene* m_scene;

    std::shared_ptr<EspShader> m_shader;

   public:
    CursorLayer(Scene* scene);

    virtual void update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

   private:
    bool gui_mouse_state_changed_event_handler(GuiMouseStateChangedEvent& event);

    void create_cursor(CursorType type, glm::vec3 position = { 0, 0, 0 });
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORLAYER_HH
