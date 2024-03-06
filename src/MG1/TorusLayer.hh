#ifndef ESPERT_SANDBOX_TORUSLAYER_HH
#define ESPERT_SANDBOX_TORUSLAYER_HH

#include "Espert.hh"
#include "Layers/Gui/GuiEvent.hh"

using namespace esp;
using namespace esp_sbx;

namespace mg1
{
  class TorusLayer : public Layer
  {
   private:
    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_render_plan;

    ModelParams m_model_params = { .m_position = true };

    std::shared_ptr<Scene> m_scene;

    bool m_pre_update{ false };

    struct
    {
      float m_R;
      float m_r;
      int m_density_theta;
      int m_density_phi;

      std::shared_ptr<Model> m_model;
      std::shared_ptr<Node> m_node;
    } m_torus;

   public:
    TorusLayer();

   private:
    virtual void pre_update(float dt) override;
    virtual void update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

    bool gui_float_param_changed_event_handler(GuiFloatParamChangedEvent& event)
    {
      bool event_handled = false;

      if (event.label_equals("R"))
      {
        m_torus.m_R   = event.get_value();
        event_handled = true;
      }
      if (event.label_equals("r"))
      {
        m_torus.m_r   = event.get_value();
        event_handled = true;
      }

      if (event_handled) { m_pre_update = true; }

      return event_handled;
    }
    bool gui_int_param_changed_event_handler(GuiIntParamChangedEvent& event)
    {
      bool event_handled = false;

      if (event.label_equals("Density - theta"))
      {
        m_torus.m_density_theta = event.get_value();
        event_handled           = true;
      }
      if (event.label_equals("Density - phi"))
      {
        m_torus.m_density_phi = event.get_value();
        event_handled         = true;
      }

      if (event_handled) { m_pre_update = true; }

      return event_handled;
    }
    /*template<typename T> bool gui_param_changed_event_handler(GuiParamChangedEvent<T>& event)
    {
      bool event_handled = false;

      if (event.label_equals("R"))
      {
        m_torus.m_R   = event.get_value();
        event_handled = true;
      }
      if (event.label_equals("r"))
      {
        m_torus.m_r   = event.get_value();
        event_handled = true;
      }
      if (event.label_equals("Density - theta"))
      {
        m_torus.m_density_theta = event.get_value();
        event_handled                = true;
      }
      if (event.label_equals("Density - phi"))
      {
        m_torus.m_density_phi = event.get_value();
        event_handled              = true;
      }

      if (event_handled) { m_pre_update = true; }

      return event_handled;
    }*/
  };
}; // namespace mg1

#endif // ESPERT_SANDBOX_TORUSLAYER_HH
