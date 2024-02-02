#ifndef LAYERS_INPUT_LAYER_HH
#define LAYERS_INPUT_LAYER_HH

#include "Espert.hh"

using namespace esp;

namespace esp_sbx
{
  class InputLayer : public Layer
  {
   public:
    InputLayer(std::vector<esp::Layer*> layers, esp::Layer* current_layer);

   private:
    std::vector<esp::Layer*> m_layers;
    esp::Layer* m_current_layer;

    inline void update(float dt) override
    {
      auto camera = Scene::get_current_camera();
      if (!camera) return;

      handle_keys(camera, dt);
      handle_mouse_move(camera, dt);
    }

    void handle_keys(Camera* camera, float dt);
    void handle_mouse_move(Camera* camera, float dt);

    void handle_event(Event& event, float dt) override;
    bool mouse_scroll_event_handler(esp::MouseScrolledEvent& event);

    void switch_to_layer(size_t i);
  };
} // namespace esp_sbx

#endif // LAYERS_INPUT_LAYER_HH
