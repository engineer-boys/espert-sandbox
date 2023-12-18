#include "Layers/Layers.hh"

// libs
#include <Espert.hh>

class SandBoxApp : public esp::Application
{
 private:
 public:
  SandBoxApp()
  {
    /* ----------- Select 1 example ----------- */
    // push_layer(new my_game::ExampleLayer());
    // push_layer(new my_game::InstancingExampleLayer());
    // push_layer(new my_game::TextureExampleLayer());
    // push_layer(new my_game::TextureDepthExampleLayer());
    // push_layer(new model_example::ModelExampleLayer());
    // push_layer(new obj_example::VikingRoomObjModelExampleLayer());
    push_layer(new obj_example::BackpackObjModelExampleLayer());
    //  push_layer(new advance_rendering_example::OffscreenRenderingLayer());
  }
};

esp::Application* esp::create_app_instance()
{
  const auto& app = new SandBoxApp();
  return app;
}
