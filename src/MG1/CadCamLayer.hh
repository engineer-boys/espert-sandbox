#ifndef ESPERT_SANDBOX_CADCAMLAYER_HH
#define ESPERT_SANDBOX_CADCAMLAYER_HH

#include "Espert.hh"
#include "Gui/GuiEvents/GuiEvents.hh"

using namespace esp;

namespace mg1
{
  class CadCamLayer : public Layer
  {
   private:
    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_render_plan;

    std::shared_ptr<OrbitCamera> m_camera;
    std::shared_ptr<Scene> m_scene;

    std::vector<std::unique_ptr<Layer>> m_children{};

    bool m_update_camera{ true };

   public:
    CadCamLayer();

    void pre_update(float dt) override;
    void update(float dt) override;
    void post_update(float dt) override;
    void handle_event(Event& event, float dt) override;

   private:
    bool mouse_moved_event_handler(esp::MouseMovedEvent& event, float dt);
    bool gui_list_box_changed_event_handler(GuiListBoxChangedEvent& event);
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CADCAMLAYER_HH
