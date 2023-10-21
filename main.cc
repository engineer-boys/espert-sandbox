#include "Layers/CubeExampleLayer.hh"

// libs
#include <Espert.hh>

using namespace my_game;

class SandBoxApp : public esp::Application
{
 private:
  std::unique_ptr<esp::EspRenderContext> m_context;

 public:
  SandBoxApp()
  {
    // TODO: perhaps it would be better to store context directly in
    //  Application and choose api here with some enum
    m_context = esp::EspRenderContext::create_and_init_vulkan(get_window());

    /* -------------------- Examples -------------------- */
    push_layer(new CubeExampleLayer());
  }
};

esp::Application* esp::createAppInstance() { return new SandBoxApp(); }
