#ifndef ESPERT_SANDBOX_OBJECTLAYER_HH
#define ESPERT_SANDBOX_OBJECTLAYER_HH

#include "Espert.hh"
#include "MG1/Events/Gui/GuiEvents.hh"
#include "MG1/Events/Object/ObjectEvents.hh"
#include "MG1/Utils.hh"

using namespace esp;

namespace mg1
{
  class ObjectLayer : public Layer
  {
   private:
    bool m_handle_mouse{ true };
    RotationAxis m_rotation_axis{ RotationNone };
    glm::vec3 m_mouse_cursor_pos{ 0, 0, 0 };

    Scene* m_scene;

    std::shared_ptr<EspShader> m_shader;

   public:
    ObjectLayer(Scene* scene);

    virtual void pre_update(float dt) override;
    virtual void update(float dt) override;
    virtual void post_update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

   private:
    bool gui_mouse_state_changed_event_handler(GuiMouseStateChangedEvent& event);
    bool gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event);
    bool mouse_moved_event_handler(MouseMovedEvent& event, float dt);
    bool mouse_scrolled_event_handler(MouseScrolledEvent& event);
    bool gui_button_clicked_event_handler(GuiButtonClickedEvent& event);
    bool cursor_pos_changed_event_handler(CursorPosChangedEvent& event);

    void create_torus(glm::vec3 position = { 0, 0, 0 });
    void create_point(glm::vec3 position = { 0, 0, 0 });
    void remove_object(ObjectInfo* info);
  };
}; // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTLAYER_HH
