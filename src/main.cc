#include "Examples/Examples.hh"
#include "Layers/Layers.hh"
#include "MG1/TorusLayer.hh"

#include "ClientPoint.hh"

class SandBoxApp : public esp::EspApplication
{
 private:
 public:
  SandBoxApp() : EspApplication({ .m_title = "My window", .m_disable_cursor = false, .m_use_gui = true })
  {
    push_layer(new esp_sbx::InputLayer());
    push_layer(new esp_sbx::GuiLayer());

    push_layer(new mg1::TorusLayer());
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
