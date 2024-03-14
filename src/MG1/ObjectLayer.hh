#ifndef ESPERT_SANDBOX_OBJECTLAYER_HH
#define ESPERT_SANDBOX_OBJECTLAYER_HH

#include "Espert.hh"
#include "MG1/Gui/GuiEvents/GuiEvents.hh"
#include "MG1/Objects/Torus.hh"

using namespace esp;

namespace mg1
{
  class ObjectLayer : public Layer
  {
   private:
    bool m_handle_mouse{ true };

    Scene* m_scene;

    std::shared_ptr<EspShader> m_shader;

    std::map<uint32_t, std::shared_ptr<Object>> m_all_objects{};

   public:
    ObjectLayer(Scene* scene);

    virtual void pre_update(float dt) override;
    virtual void update(float dt) override;
    virtual void post_update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

   private:
    bool gui_float_param_changed_event_handler(GuiInputFloatChangedEvent& event);
    bool gui_int_param_changed_event_handler(GuiInputIntChangedEvent& event);
    bool gui_mouse_state_changed_event_handler(GuiMouseStateChangedEvent& event);

    bool mouse_moved_event_handler(MouseMovedEvent& event, float dt);
    bool mouse_scrolled_event_handler(MouseScrolledEvent& event);

    void create_initial_scene(); // TODO: remove
  };
}; // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTLAYER_HH
