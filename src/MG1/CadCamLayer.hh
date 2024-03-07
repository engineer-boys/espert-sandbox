#ifndef ESPERT_SANDBOX_CADCAMLAYER_HH
#define ESPERT_SANDBOX_CADCAMLAYER_HH

#include "Espert.hh"

#include "Layers/Gui/GuiLayer.hh"
#include "TorusLayer.hh"

using namespace esp;

namespace mg1
{
  class CadCamLayer : public Layer
  {
   private:
    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_render_plan;

    std::shared_ptr<Scene> m_scene;

    std::vector<std::unique_ptr<Layer>> m_children{};

   public:
    CadCamLayer();

    void pre_update(float dt) override;
    void update(float dt) override;
    void post_update(float dt) override;
    void handle_event(Event& event, float dt) override;
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CADCAMLAYER_HH
