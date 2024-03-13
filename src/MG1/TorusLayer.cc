#include "TorusLayer.hh"
#include "Objects/ObjectEvent.hh"

namespace mg1
{
  TorusLayer::TorusLayer(Scene* scene) : m_scene{ scene }
  {
    // create shader
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4));

      m_shader = ShaderSystem::acquire("Shaders/MG1/TorusLayer/shader");
      // m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({ TorusInfo::s_model_params.get_vertex_layouts() });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->set_rasterizer_settings({ .m_polygon_mode = ESP_POLYGON_MODE_LINE, .m_cull_mode = ESP_CULL_MODE_NONE });
      m_shader->build_worker();
    }

    m_all_objects[0] = std::make_shared<Object>("None");
    create_initial_scene();
  }

  void TorusLayer::pre_update(float dt)
  {
    for (auto& kv : m_all_objects)
    {
      auto object = kv.second;
      if (!object->is_selected()) { continue; }
      object->pre_update(dt);
    }
  }

  void TorusLayer::update(float dt)
  {
    for (auto& kv : m_all_objects)
    {
      auto object = kv.second;
      object->update(dt);
    }
  }

  void TorusLayer::post_update(float dt)
  {
    static bool temp = true;
    if (temp)
    {
      for (auto& kv : m_all_objects)
      {
        auto object = kv.second;
        ObjectAddedEvent event{ { object->get_id(), object->get_name(), object->get_state_handle() } };
        post_event(event);
      }
      temp = false;
    }
  }

  void TorusLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<GuiFloatParamChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(TorusLayer::gui_float_param_changed_event_handler));
    Event::try_handler<GuiIntParamChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(TorusLayer::gui_int_param_changed_event_handler));
    Event::try_handler<GuiMouseStateChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(TorusLayer::gui_mouse_state_changed_event_handler));

    if (m_handle_mouse)
    {
      Event::try_handler<MouseMovedEvent>(event, ESP_BIND_EVENT_FOR_FUN(TorusLayer::mouse_moved_event_handler, dt));
      Event::try_handler<MouseScrolledEvent>(event, ESP_BIND_EVENT_FOR_FUN(TorusLayer::mouse_scrolled_event_handler));
    }
  }

  bool TorusLayer::gui_float_param_changed_event_handler(GuiFloatParamChangedEvent& event)
  {
    for (auto& kv : m_all_objects)
    {
      auto object = kv.second;
      if (!object->is_selected()) { continue; }
      object->handle_event(event);
    }
    return true;
  }

  bool TorusLayer::gui_int_param_changed_event_handler(GuiIntParamChangedEvent& event)
  {
    for (auto& kv : m_all_objects)
    {
      auto object = kv.second;
      if (!object->is_selected()) { continue; }
      object->handle_event(event);
    }

    if (event.label_equals("Rotation axis")) { Object::set_rotation_axis(event.get_value()); }

    return true;
  }

  bool TorusLayer::gui_mouse_state_changed_event_handler(mg1::GuiMouseStateChangedEvent& event)
  {
    m_handle_mouse = !(bool)event.get_state();
    return true;
  }

  bool TorusLayer::mouse_moved_event_handler(esp::MouseMovedEvent& event, float dt)
  {
    auto camera = Scene::get_current_camera();
    if (camera == nullptr) { return false; }

    camera->look_at({ event.get_x(), event.get_y() }, dt);

    for (auto& kv : m_all_objects)
    {
      auto object = kv.second;
      if (!object->is_selected()) { continue; }
      object->handle_event(event, dt);
    }

    return true;
  }

  bool TorusLayer::mouse_scrolled_event_handler(esp::MouseScrolledEvent& event)
  {
    for (auto& kv : m_all_objects)
    {
      auto object = kv.second;
      if (!object->is_selected()) { continue; }
      object->handle_event(event);
    }

    return true;
  }

  void TorusLayer::create_initial_scene()
  {
    {
      Torus torus{};

      auto entity = m_scene->create_entity();
      entity->add_component<ModelComponent>(torus.get_model(), m_shader);

      torus.get_node()->attach_entity(entity);
      torus.get_node()->translate({ 2, 0, -5 });

      m_scene->get_root().add_child(torus.get_node());

      m_all_objects[torus.get_id()] = std::make_shared<Torus>(torus);
    }

    {
      Torus torus{};

      auto entity = m_scene->create_entity();
      entity->add_component<ModelComponent>(torus.get_model(), m_shader);

      torus.get_node()->attach_entity(entity);
      torus.get_node()->translate({ -2, 0, -5 });

      m_scene->get_root().add_child(torus.get_node());

      m_all_objects[torus.get_id()] = std::make_shared<Torus>(torus);
    }

    {
      Torus torus{};

      auto entity = m_scene->create_entity();
      entity->add_component<ModelComponent>(torus.get_model(), m_shader);

      torus.get_node()->attach_entity(entity);
      torus.get_node()->translate({ 0, 0, -10 });

      m_scene->get_root().add_child(torus.get_node());

      m_all_objects[torus.get_id()] = std::make_shared<Torus>(torus);
    }
  }
} // namespace mg1