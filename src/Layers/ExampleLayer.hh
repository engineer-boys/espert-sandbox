#ifndef LAYERS_CUBE_EXAMPLE_LAYER_H
#define LAYERS_CUBE_EXAMPLE_LAYER_H

// libs
#include "Espert.hh"

namespace my_game
{
  class ExampleLayer : public esp::Layer
  {
    esp::renderer::Camera m_camera{};

   public:
    ExampleLayer() {}

   private:
    virtual ~ExampleLayer() override {}

    virtual void attach() override {}

    virtual void detach() override {}

    virtual void update() override
    {
    }

    virtual void handle_event(esp::Event& event) override
    {
      esp::Event::try_hanlder<esp::KeyPressedEvent>(event,
                                                    ESP_BIND_EVENT_FOR_FUN(ExampleLayer::my_crazy_event_handler));
    }

    bool my_crazy_event_handler(esp::KeyPressedEvent& event)
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
} // namespace my_game

#endif // LAYERS_CUBE_EXAMPLE_LAYER_H
