#include "InputLayer.hh"

namespace esp_sbx
{
  InputLayer::InputLayer(std::vector<esp::Layer*> layers, esp::Layer* current_layer) :
      m_layers(layers), m_current_layer(current_layer)
  {
  }

  void InputLayer::handle_keys(Camera* camera, float dt)
  {
    if (EspInput::is_key_pressed(ESP_KEY_W)) { camera->move(Camera::FORWARD, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_S)) { camera->move(Camera::BACKWARD, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_A)) { camera->move(Camera::LEFT, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_D)) { camera->move(Camera::RIGHT, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_SPACE)) { camera->move(Camera::UP, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_LEFT_SHIFT)) { camera->move(Camera::DOWN, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_1)) { switch_to_layer(0); }
    if (EspInput::is_key_pressed(ESP_KEY_2)) { switch_to_layer(1); }
    if (EspInput::is_key_pressed(ESP_KEY_3)) { switch_to_layer(2); }
    if (EspInput::is_key_pressed(ESP_KEY_4)) { switch_to_layer(3); }
    if (EspInput::is_key_pressed(ESP_KEY_5)) { switch_to_layer(4); }
    if (EspInput::is_key_pressed(ESP_KEY_6)) { switch_to_layer(5); }
    if (EspInput::is_key_pressed(ESP_KEY_7)) { switch_to_layer(6); }
    if (EspInput::is_key_pressed(ESP_KEY_8)) { switch_to_layer(7); }
    if (EspInput::is_key_pressed(ESP_KEY_9)) { switch_to_layer(8); }
    if (EspInput::is_key_pressed(ESP_KEY_0)) { switch_to_layer(9); }
    if (EspInput::is_key_pressed(ESP_KEY_MINUS)) { switch_to_layer(10); }
    if (EspInput::is_key_pressed(ESP_KEY_EQUAL)) { switch_to_layer(11); }
  }

  void InputLayer::handle_mouse_move(esp::Camera* camera, float dt)
  {
    camera->look_at({ EspInput::get_mouse_x(), EspInput::get_mouse_y() }, dt);
  }

  void InputLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<MouseScrolledEvent>(event, ESP_BIND_EVENT_FOR_FUN(InputLayer::mouse_scroll_event_handler));
  }

  bool InputLayer::mouse_scroll_event_handler(esp::MouseScrolledEvent& event)
  {
    auto camera = Scene::get_current_camera();

    if (camera == nullptr) { return false; }

    camera->set_fov(camera->get_fov() - glm::radians(event.get_offset_y()));

    return true;
  }

  void InputLayer::switch_to_layer(size_t i)
  {
    if (i < m_layers.size())
    {
      m_current_layer->detach();
      m_current_layer = m_layers[i];
      m_current_layer->attach();
    }
  }
} // namespace esp_sbx