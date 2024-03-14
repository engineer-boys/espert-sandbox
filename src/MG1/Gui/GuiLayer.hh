#ifndef ESPERT_SANDBOX_GUILAYER_HH
#define ESPERT_SANDBOX_GUILAYER_HH

#include "Espert.hh"
#include "GuiEvents/GuiEvents.hh"
#include "GuiFields/GuiFields.hh"
#include "MG1/Objects/ObjectEvents/ObjectEvents.hh"
#include "MG1/Objects/ObjectUtils.hh"

using namespace esp;

namespace mg1
{
  class GuiLayer : public Layer
  {
   private:
    static constexpr ImVec2 OFFSET{ 10, 10 };

    MouseState m_mouse_state{ NotCaptured };

    struct
    {
      std::unique_ptr<GuiInputFloat> m_R;
      std::unique_ptr<GuiInputFloat> m_r;
      std::unique_ptr<GuiInputInt> m_density_theta;
      std::unique_ptr<GuiInputInt> m_density_phi;
    } m_torus_params;

    std::unique_ptr<GuiRadioButton> m_rotation_axis_field;

    std::unique_ptr<GuiListBox> m_objects_list_box;

   public:
    GuiLayer();

    void update(float dt) override;
    void handle_event(Event& event, float dt) override;

   private:
    void update_mouse_state();
    void help_marker(const char* desc);

    template<typename T> void update_param(GuiField<T>& param)
    {
      param.render();
      if (param.changed()) { post_event(*(param.create_event())); }
    }

    bool object_added_event_handler(ObjectAddedEvent& event);
    // bool object_removed_event_handler(ObjectRemovedEvent& event);
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUILAYER_HH
