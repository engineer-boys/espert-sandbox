#include "Examples/Examples.hh"
#include "Layers/Layers.hh"

#include "ClientPoint.hh"

class SandBoxApp : public esp::EspApplication
{
 private:
 public:
  SandBoxApp() : EspApplication({ .m_title = "My window", .m_disable_cursor = true })
  {
    auto layers = std::vector<esp::Layer*>();
    layers.emplace_back(new my_game::ExampleLayer());
    layers.emplace_back(new my_game::InstancingExampleLayer());
    layers.emplace_back(new my_game::TextureExampleLayer());
    layers.emplace_back(new my_game::TextureDepthExampleLayer());
    layers.emplace_back(new obj_example::VikingRoomObjModelExampleLayer());
    layers.emplace_back(new obj_example::BackpackObjModelExampleLayer());
    layers.emplace_back(new advance_rendering_example::SkyBoxLayer());
    layers.emplace_back(new advance_rendering_example::OffscreenRenderingLayer());
    layers.emplace_back(new advance_rendering2_example::PBRBasicLayer());
    layers.emplace_back(new advance_rendering2_example::PBRIBLLayer());
    layers.emplace_back(new advance_model::AdvanceModelLayer());
    layers.emplace_back(new advance_model::AnimatedModelLayer());
    layers[4]->attach();

    push_layer(new esp_sbx::InputLayer(layers, layers[4]));

    for (const auto& layer : layers)
    {
      push_layer(layer);
    }

    /* ----------- Select 1 example ----------- */
    // push_layer(new my_game::ExampleLayer());
    // push_layer(new my_game::InstancingExampleLayer());
    // push_layer(new my_game::TextureExampleLayer());
    // push_layer(new my_game::TextureDepthExampleLayer());
    // push_layer(new model_example::ModelExampleLayer());
    // push_layer(new obj_example::VikingRoomObjModelExampleLayer());
    // push_layer(new obj_example::BackpackObjModelExampleLayer());
    // push_layer(new advance_rendering_example::SkyBoxLayer());
    // push_layer(new advance_rendering_example::OffscreenRenderingLayer());

    // push_layer(new advance_rendering2_example::PBRBasicLayer());
    // push_layer(new advance_rendering2_example::PBRIBLLayer());
    // push_layer(new advance_rendering2_example::PBRTexturedLayer());

    // push_layer(new advance_model::AdvanceModelLayer());
    // push_layer(new advance_model::AnimatedModelLayer());
  }

  void virtual update(float dt) override
  {
    if (EspInput::is_key_pressed(ESP_KEY_ESCAPE)) { m_running = false; }
  }
};

esp::EspApplication* esp::create_app_instance()
{
  const auto& app = new SandBoxApp();
  return app;
}
