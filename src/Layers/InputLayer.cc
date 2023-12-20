#include "InputLayer.hh"

namespace esp_sbx
{
  void InputLayer::handle_keys(Camera* camera, float dt)
  {
    if (EspInput::is_key_pressed(ESP_KEY_W)) { camera->move(Camera::FORWARD, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_S)) { camera->move(Camera::BACKWARD, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_A)) { camera->move(Camera::LEFT, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_D)) { camera->move(Camera::RIGHT, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_SPACE)) { camera->move(Camera::UP, dt); }
    if (EspInput::is_key_pressed(ESP_KEY_LEFT_SHIFT)) { camera->move(Camera::DOWN, dt); }
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
} // namespace esp_sbx