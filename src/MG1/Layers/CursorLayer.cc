#include "CursorLayer.hh"
#include "MG1/Common/InitInfo.hh"
#include "MG1/Events/Object/ObjectEvents.hh"

namespace mg1
{
  CursorLayer::CursorLayer(Scene* scene) : m_scene{ scene }
  {
    // create shader
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::vec3)); // cursor pos
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4)); // vp

      m_shader = ShaderSystem::acquire("Shaders/MG1/CursorLayer/shader");
      m_shader->set_attachment_formats({ EspBlockFormat::ESP_FORMAT_R8G8B8A8_UNORM });
      m_shader->set_rasterizer_settings(
          { .m_polygon_mode = ESP_POLYGON_MODE_LINE, .m_cull_mode = ESP_CULL_MODE_NONE, .m_line_width = 5.f });
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({ CursorInit::S_MODEL_PARAMS.get_vertex_layouts() });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->build_worker();
    }
  }

  void CursorLayer::update(float dt)
  {
    if (!m_update) return;

    auto camera  = Scene::get_current_camera();
    glm::mat4 vp = camera->get_projection() * camera->get_view();

    for (auto&& [entity, cursor, model] : m_scene->get_view<CursorComponent, ModelComponent>())
    {
      if (!cursor.get_info()->selected())
      {
        if (m_update_when_mouse_pressed)
        {
          if (!m_gui_captured) { cursor.update_when_mouse_pressed(); }
        }
        else { cursor.update(); }
      }
      CursorPosChangedEvent event{ cursor.get_info()->m_type,
                                   cursor.get_info()->m_position,
                                   cursor.get_delta_position() };
      post_event(event);

      auto& uniform_manager = model.get_uniform_manager();
      glm::vec3 cursor_pos  = cursor.get_position();
      uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(glm::vec3), &cursor_pos);
      uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(glm::mat4), &vp);
    }
  }

  void CursorLayer::post_update(float dt)
  {
    static bool first_loop = true;
    if (first_loop)
    {
      // initial scene
      create_cursor(CursorType::Mouse);
      first_loop = false;
    }
  }

  void CursorLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<GuiMouseStateChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(CursorLayer::gui_mouse_state_changed_event_handler));
    Event::try_handler<ObjectMassCentreChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(CursorLayer::object_mass_centre_changed_event_handler));
    Event::try_handler<GuiSelectableChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(CursorLayer::gui_selectable_changed_event_handler));
    Event::try_handler<MouseMovedEvent>(event, ESP_BIND_EVENT_FOR_FUN(CursorLayer::mouse_moved_event_handler, dt));
    Event::try_handler<MouseScrolledEvent>(event, ESP_BIND_EVENT_FOR_FUN(CursorLayer::mouse_scrolled_event_handler));
    Event::try_handler<CursorPosChangedEvent>(event,
                                              ESP_BIND_EVENT_FOR_FUN(CursorLayer::cursor_pos_changed_event_handler));
  }

  bool CursorLayer::gui_mouse_state_changed_event_handler(mg1::GuiMouseStateChangedEvent& event)
  {
    m_gui_captured = (bool)event.get_state();
    // m_update = !(bool)event.get_state();
    //    if (m_update) { push_cursor(); }
    //    else { pop_cursor(); }
    return false;
  }

  bool CursorLayer::gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event)
  {
    /*if (event == GuiLabel::action_none)
    {
      m_rotation_axis = RotationNone;
      m_scale_axis    = ScaleNone;
    }*/
    if (event == GuiLabel::action_rot_ox) { m_rotation_axis = event.get_value() ? RotationOX : RotationNone; }
    if (event == GuiLabel::action_rot_oy) { m_rotation_axis = event.get_value() ? RotationOY : RotationNone; }
    if (event == GuiLabel::action_rot_oz) { m_rotation_axis = event.get_value() ? RotationOZ : RotationNone; }
    if (event == GuiLabel::action_scale) { m_scale_axis = event.get_value() ? Scale : ScaleNone; }
    if (event == GuiLabel::action_scale_ox) { m_scale_axis = event.get_value() ? ScaleOX : ScaleNone; }
    if (event == GuiLabel::action_scale_oy) { m_scale_axis = event.get_value() ? ScaleOY : ScaleNone; }
    if (event == GuiLabel::action_scale_oz) { m_scale_axis = event.get_value() ? ScaleOZ : ScaleNone; }
    if (event == GuiLabel::action_set_cursor_pos) { m_update_when_mouse_pressed = event.get_value(); }

    return false;
  }

  bool CursorLayer::mouse_moved_event_handler(esp::MouseMovedEvent& event, float dt)
  {
    for (auto&& [entity, cursor] : m_scene->get_view<CursorComponent>())
    {
      cursor.handle_event(event, dt, m_rotation_axis);
    }

    return false;
  }

  bool CursorLayer::mouse_scrolled_event_handler(esp::MouseScrolledEvent& event)
  {
    for (auto&& [entity, cursor] : m_scene->get_view<CursorComponent>())
    {
      cursor.handle_event(event, m_scale_axis);
    }

    return false;
  }

  bool CursorLayer::object_mass_centre_changed_event_handler(mg1::ObjectMassCentreChangedEvent& event)
  {
    if (!(event == ObjectLabel::object_mass_centre_changed_event)) { return false; }

    for (auto&& [entity, cursor] : m_scene->get_view<CursorComponent>())
    {
      if (cursor.is_type(CursorType::Object))
      {
        if (event.create())
        {
          cursor.get_node()->set_translation(event.get_position(), action::ESP_RELATIVE);
          return false;
        }
        else
        {
          remove_cursor(cursor.get_node(), cursor.get_info());
          return false;
        }
      }
    }

    if (event.create()) { create_cursor(CursorType::Object, event.get_position()); }

    return false;
  }

  bool CursorLayer::cursor_pos_changed_event_handler(CursorPosChangedEvent& event)
  {
    if (!(event == ObjectLabel::cursor_pos_changed_event && event.is_type(CursorType::Mouse))) { return false; }

    for (auto&& [entity, cursor] : m_scene->get_view<CursorComponent>())
    {
      if (cursor.is_type(CursorType::Object))
      {
        if (EspInput::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT))
        {
          auto d_pos = event.get_delta_position();
          cursor.get_node()->translate({ d_pos.x, 0, 0 });
          if (EspInput::is_key_pressed(GLFW_KEY_Y)) { cursor.get_node()->translate({ 0, -d_pos.z, 0 }); }
          if (EspInput::is_key_pressed(GLFW_KEY_Z)) { cursor.get_node()->translate({ 0, 0, d_pos.z }); }
        }
        break;
      }
    }

    return false;
  }

  void CursorLayer::create_cursor(mg1::CursorType type, glm::vec3 position)
  {
    auto entity = m_scene->create_entity();

    entity->add_component<CursorComponent>(entity->get_id(), type, position);
    auto& cursor = entity->get_component<CursorComponent>();

    auto [vertices, indices] = CursorComponent::construct();
    auto model               = std::make_shared<Model>(vertices,
                                         indices,
                                         std::vector<std::shared_ptr<EspTexture>>{},
                                         CursorInit::S_MODEL_PARAMS);
    entity->add_component<ModelComponent>(model, m_shader);

    cursor.get_node()->attach_entity(entity);
    cursor.get_node()->translate(position);

    m_scene->get_root().add_child(cursor.get_node());
  }

  void CursorLayer::remove_cursor(Node* node, ObjectInfo* info)
  {
    auto parent = node->get_parent();
    parent->rebase_child(nullptr, node);

    EspJob::done_all_jobs();
    parent->remove_child(node);
    m_scene->destroy_entity(info->m_id);
    ObjectRemovedEvent event{ info->m_name };
    post_event(event);
  }

  void CursorLayer::push_cursor()
  {
    for (auto&& [entity, cursor] : m_scene->get_view<CursorComponent>())
    {
      m_scene->get_root().add_child(cursor.get_node());
    }
  }

  void CursorLayer::pop_cursor()
  {
    for (auto&& [entity, cursor] : m_scene->get_view<CursorComponent>())
    {
      m_scene->get_root().remove_child(cursor.get_node());
    }
  }

} // namespace mg1