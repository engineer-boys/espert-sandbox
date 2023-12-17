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
  }

  void set_background_color() { m_frame_manager->set_clear_color(glm::vec4{ .1f, .1f, .3f, 1.f }); }
};

esp::Application* esp::create_app_instance()
{
  const auto& app = new SandBoxApp();
  app->set_background_color();
  return app;
}
