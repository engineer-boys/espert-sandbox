#include "Layers/ExampleLayer.hh"
#include "Layers/InstancingExampleLayer.hh"

// libs
#include <Espert.hh>

class SandBoxApp : public esp::Application
{
 private:
 public:
  SandBoxApp()
  {
    /* ----------- Select 1 example ----------- */
    push_layer(new my_game::ExampleLayer());
    // push_layer(new my_game::InstancingExampleLayer());
  }
};

esp::Application* esp::create_app_instance() { return new SandBoxApp(); }
