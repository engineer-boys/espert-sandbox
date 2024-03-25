#ifndef ESPERT_SANDBOX_CURSORLAYER_HH
#define ESPERT_SANDBOX_CURSORLAYER_HH

#include "Espert.hh"
#include "MG1/Components/CursorComponent.hh"
#include "MG1/Events/Gui/GuiEvents.hh"
#include "MG1/Events/Object/ObjectEvents.hh"

using namespace esp;

namespace mg1
{
  class CursorLayer : public Layer
  {
   private:
    bool m_update{ true };
    bool m_gui_captured{ false };
    bool m_update_when_mouse_pressed{ false };
    RotationAxis m_rotation_axis{ RotationNone };
    ScaleAxis m_scale_axis{ ScaleNone };

    Scene* m_scene;

    std::shared_ptr<EspShader> m_shader;

   public:
    CursorLayer(Scene* scene);

    virtual void update(float dt) override;
    virtual void post_update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

   private:
    bool gui_mouse_state_changed_event_handler(GuiMouseStateChangedEvent& event);
    bool object_mass_centre_changed_event_handler(ObjectMassCentreChangedEvent& event);
    bool gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event);
    bool mouse_moved_event_handler(esp::MouseMovedEvent& event, float dt);
    bool mouse_scrolled_event_handler(MouseScrolledEvent& event);
    bool cursor_pos_changed_event_handler(CursorPosChangedEvent& event);

    void create_cursor(CursorType type, glm::vec3 position = { 0, 0, 0 });
    void remove_cursor(Node* node, ObjectInfo* info);
    void push_cursor();
    void pop_cursor();
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORLAYER_HH
