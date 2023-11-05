#include "Layers/ExampleLayer.hh"
#include "Layers/InstancingExampleLayer.hh"
#include "Layers/TextureExampleLayer.hh"

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
    push_layer(new my_game::TextureExampleLayer());
  }

  void set_background_color() { m_frame_manager->set_clear_color(glm::vec4{ .1f, .1f, .3f, 1.f }); }
};

esp::Application* esp::create_app_instance()
{
  const auto& app = new SandBoxApp();
  app->set_background_color();
  return app;
}
