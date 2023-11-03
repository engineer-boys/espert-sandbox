#include "Layers/ExampleInstancingLayer.hh"

// libs
#include <Espert.hh>

class SandBoxApp : public esp::Application
{
 private:

 public:
  SandBoxApp() { push_layer(new my_game::ExampleInstancingLayer()); }
};

esp::Application* esp::create_app_instance() { return new SandBoxApp(); }
