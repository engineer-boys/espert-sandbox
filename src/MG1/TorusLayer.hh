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
    int m_rotation_axis;

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

    bool gui_float_param_changed_event_handler(GuiFloatParamChangedEvent& event);
    bool gui_int_param_changed_event_handler(GuiIntParamChangedEvent& event);

    bool mouse_moved_event_handler(MouseMovedEvent& event, float dt);
    bool mouse_scrolled_event_handler(MouseScrolledEvent& event);
  };
}; // namespace mg1

#endif // ESPERT_SANDBOX_TORUSLAYER_HH
