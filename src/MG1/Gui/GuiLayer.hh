#ifndef ESPERT_SANDBOX_GUILAYER_HH
#define ESPERT_SANDBOX_GUILAYER_HH

#include "Espert.hh"
#include "GuiEvent.hh"
#include "GuiParam.hh"
#include "MG1/Objects/ObjectEvent.hh"
#include "MG1/Objects/ObjectUtils.hh"

using namespace esp;

namespace mg1
{
  template<typename T> using Param = std::unique_ptr<GuiParam<T>>;

  class GuiLayer : public Layer
  {
   private:
    static constexpr ImVec2 OFFSET{ 10, 10 };

    MouseState m_mouse_state{ NOT_CAPTURED };

    struct
    {
      Param<float> m_R;
      Param<float> m_r;
      Param<int> m_density_theta;
      Param<int> m_density_phi;
    } m_torus_params;

    Param<int> m_rotation_axis_field;

    std::unique_ptr<GuiStringListBoxField> m_objects_list_box;

   public:
    GuiLayer();

    void update(float dt) override;
    void handle_event(Event& event, float dt) override;

   private:
    void update_mouse_state();
    void help_marker(const char* desc);

    template<typename T> void update_param(GuiParam<T>& param)
    {
      param.render();
      if (param.changed()) { post_event(*(param.create_event())); }
    }

    bool object_added_event_handler(ObjectAddedEvent& event);
    // bool object_removed_event_handler(ObjectRemovedEvent& event);
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUILAYER_HH
