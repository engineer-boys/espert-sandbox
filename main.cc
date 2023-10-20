#include "MyRenderSystem.hh"

// libs
#include <Espert.hh>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class CrazyLayer : public esp::Layer
{
  esp::renderer::Camera m_camera{};

  my_game::MyGameObject m_cube{};
  my_game::MyRenderSystem m_render_system{};

 public:
  CrazyLayer()
  {
    std::shared_ptr<esp::ModelComponent> model = my_game::create_cube_model(
            esp::EspRenderContext::get_device(),
            glm::vec3{0.0f, 0.0f, 0.0f});

    m_cube.m_model = model;
    m_cube.m_transform.m_translation = {0.0f, 0.0f, 2.5f};
    m_cube.m_transform.m_scale = {0.5f, 0.5f, 0.5f};
  }

 private:
  virtual ~CrazyLayer() override {}

  virtual void attach() override {}

  virtual void detach() override {}

  virtual void update() override
  {
    auto& scheduler = esp::EspRenderContext::get_scheduler();

    m_camera.set_perspective(scheduler.get_aspect_ratio());

    if (auto command_buffer = scheduler.begin_frame())
    {
            scheduler.begin_swap_chain_render_pass(command_buffer);
            m_render_system.render(command_buffer, m_cube, m_camera);
            scheduler.end_swap_chain_render_pass(command_buffer);
            scheduler.end_frame();
    }

    auto& rot_y = m_cube.m_transform.m_rotation.y;
    auto& rot_x = m_cube.m_transform.m_rotation.x;
    rot_y = glm::mod(rot_x + 0.01f, glm::two_pi<float>());
    rot_x = glm::mod(rot_y + 0.01f / 2, glm::two_pi<float>());
  }

  virtual void handle_event(esp::Event &event) override
  {
      esp::Event::
          try_hanlder<esp::KeyPressedEvent>(event, ESP_BIND_EVENT_FOR_FUN(CrazyLayer::my_crazy_event_handler));
  }

  bool my_crazy_event_handler(esp::KeyPressedEvent &event)
  {
    // implementation ...
    ESP_INFO("You pressed the button {0}", event.get_code());

    /*
        true - propagate to lower layers
        false - this is the last layer of this event
    */
    return true;
  }
};

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
      push_layer(new CrazyLayer());
    }
};

esp::Application *esp::createAppInstance()
{
  return new SandBoxApp();
}
