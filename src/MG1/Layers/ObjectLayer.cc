#include "ObjectLayer.hh"
#include "MG1/Common/InitInfo.hh"
#include "MG1/Components/Components.hh"

namespace mg1
{
  ObjectLayer::ObjectLayer(Scene* scene) : m_scene{ scene }
  {
    // create shader
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4));
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(glm::vec3));

      m_shader = ShaderSystem::acquire("Shaders/MG1/ObjectLayer/shader");
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({ TorusInit::S_MODEL_PARAMS.get_vertex_layouts() });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->set_rasterizer_settings({ .m_polygon_mode = ESP_POLYGON_MODE_LINE, .m_cull_mode = ESP_CULL_MODE_NONE });
      m_shader->build_worker();
    }
  }

  void ObjectLayer::pre_update(float dt)
  {
    auto torus_view = m_scene->m_registry.view<TorusComponent, ModelComponent>();
    for (auto&& [entity, torus, model] : torus_view.each())
    {
      if (torus.get_info()->removed()) { remove_object(torus.get_info()); }
      else if (torus.get_info()->m_dirty)
      {
        auto [vertices, indices] = torus.reconstruct();
        model.get_model().set_vertex_buffer(vertices);
        model.get_model().set_index_buffer(indices, 0);
      }
    }

    auto point_view = m_scene->m_registry.view<PointComponent, ModelComponent>();
    for (auto&& [entity, point, model] : point_view.each())
    {
      if (point.get_info()->removed()) { remove_object(point.get_info()); }
      else if (point.get_info()->m_dirty)
      {
        auto [vertices, indices] = point.reconstruct();
        model.get_model().set_vertex_buffer(vertices);
        model.get_model().set_index_buffer(indices, 0);
      }
    }
  }

  void ObjectLayer::update(float dt)
  {
    auto camera = Scene::get_current_camera();

    glm::vec3 mass_sum   = { 0, 0, 0 };
    int selected_objects = 0;

    auto torus_view = m_scene->m_registry.view<TorusComponent, ModelComponent>();
    for (auto&& [entity, torus, model] : torus_view.each())
    {
      auto& uniform_manager = model.get_uniform_manager();
      glm::mat4 mvp         = camera->get_projection() * camera->get_view() * torus.get_node()->get_model_mat();
      uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &mvp);

      glm::vec3 color = torus.get_info()->selected() ? ObjectConstants::selected_color : ObjectConstants::default_color;
      uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(glm::vec3), &color);

      if (torus.get_info()->selected())
      {
        mass_sum += torus.get_node()->get_translation();
        selected_objects++;
      }
    }

    auto point_view = m_scene->m_registry.view<PointComponent, ModelComponent>();
    for (auto&& [entity, point, model] : point_view.each())
    {
      auto& uniform_manager = model.get_uniform_manager();
      glm::mat4 mvp         = camera->get_projection() * camera->get_view() * point.get_node()->get_model_mat();
      uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &mvp);

      glm::vec3 color = point.get_info()->selected() ? ObjectConstants::selected_color : ObjectConstants::default_color;
      uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(glm::vec3), &color);

      point.check_if_clicked();
      if (point.get_info()->selected())
      {
        mass_sum += point.get_node()->get_translation();
        selected_objects++;
      }
    }

    if (selected_objects > 0)
    {
      glm::vec3 new_mass_centre = mass_sum / (float)selected_objects;
      if (new_mass_centre != m_mass_centre)
      {
        m_mass_centre = new_mass_centre;
        ObjectMassCentreChangedEvent event{ true, m_mass_centre };
        post_event(event);
      }
    }
    else if (m_mass_centre != glm::vec3{ 0, 0, 0 })
    {
      ObjectMassCentreChangedEvent event{ false };
      post_event(event);
      m_mass_centre = { 0, 0, 0 };
    }
  }

  void ObjectLayer::post_update(float dt)
  {
    static bool first_loop = true;
    if (first_loop)
    {
      // initial scene
      create_torus({ 2, 0, -5 });
      create_torus({ -2, 0, -5 });
      create_point({ 1, 1, -1 });
      create_point({ 0, 1, -1 });
      create_point({ -1, 1, -1 });
      first_loop = false;
    }
  }

  void ObjectLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<GuiMouseStateChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(ObjectLayer::gui_mouse_state_changed_event_handler));
    Event::try_handler<GuiSelectableChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(ObjectLayer::gui_selectable_changed_event_handler));
    if (m_handle_mouse)
    {
      Event::try_handler<MouseMovedEvent>(event, ESP_BIND_EVENT_FOR_FUN(ObjectLayer::mouse_moved_event_handler, dt));
      Event::try_handler<MouseScrolledEvent>(event, ESP_BIND_EVENT_FOR_FUN(ObjectLayer::mouse_scrolled_event_handler));
    }
    Event::try_handler<GuiButtonClickedEvent>(event,
                                              ESP_BIND_EVENT_FOR_FUN(ObjectLayer::gui_button_clicked_event_handler));
    Event::try_handler<CursorPosChangedEvent>(event,
                                              ESP_BIND_EVENT_FOR_FUN(ObjectLayer::cursor_pos_changed_event_handler));
  }

  bool ObjectLayer::gui_mouse_state_changed_event_handler(mg1::GuiMouseStateChangedEvent& event)
  {
    m_handle_mouse = !(bool)event.get_state();
    return false;
  }

  bool ObjectLayer::mouse_moved_event_handler(esp::MouseMovedEvent& event, float dt)
  {
    auto view = m_scene->m_registry.view<TorusComponent>();
    for (auto&& [entity, torus] : view.each())
    {
      if (torus.get_info()->selected()) { torus.handle_event(event, dt, m_rotation_axis); }
    }

    return true;
  }

  bool ObjectLayer::mouse_scrolled_event_handler(esp::MouseScrolledEvent& event)
  {
    auto view = m_scene->m_registry.view<TorusComponent>();
    for (auto&& [entity, torus] : view.each())
    {
      if (torus.get_info()->selected()) { torus.handle_event(event); }
    }

    return true;
  }

  bool ObjectLayer::gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event)
  {
    if (event == GuiLabel::action_none) { m_rotation_axis = RotationNone; }
    if (event == GuiLabel::action_rot_ox) { m_rotation_axis = RotationOX; }
    if (event == GuiLabel::action_rot_oy) { m_rotation_axis = RotationOY; }
    if (event == GuiLabel::action_rot_oz) { m_rotation_axis = RotationOZ; }

    return true;
  }

  bool ObjectLayer::gui_button_clicked_event_handler(mg1::GuiButtonClickedEvent& event)
  {
    if (event == GuiLabel::create_torus_button) { create_torus(m_mouse_cursor_pos); }
    if (event == GuiLabel::create_point_button) { create_point(m_mouse_cursor_pos); }

    return true;
  }

  bool ObjectLayer::cursor_pos_changed_event_handler(CursorPosChangedEvent& event)
  {
    if (!(event == ObjectLabel::cursor_pos_changed_event && event.is_type(CursorType::Mouse))) { return false; }
    m_mouse_cursor_pos = event.get_position();

    auto torus_view = m_scene->m_registry.view<TorusComponent>();
    for (auto&& [entity, torus] : torus_view.each())
    {
      if (torus.get_info()->selected()) { torus.handle_event(event); }
    }

    auto point_view = m_scene->m_registry.view<PointComponent>();
    for (auto&& [entity, torus] : point_view.each())
    {
      if (torus.get_info()->selected()) { torus.handle_event(event); }
    }

    return true;
  }

  void ObjectLayer::create_torus(glm::vec3 position)
  {
    auto entity = m_scene->create_entity();

    entity->add_component<TorusComponent>(entity->get_id());
    auto& torus = entity->get_component<TorusComponent>();

    auto [vertices, indices] = torus.reconstruct();
    auto model               = std::make_shared<Model>(vertices,
                                         indices,
                                         std::vector<std::shared_ptr<EspTexture>>{},
                                         TorusInit::S_MODEL_PARAMS);
    entity->add_component<ModelComponent>(model, m_shader);

    torus.get_node()->attach_entity(entity);
    torus.get_node()->translate(position);

    m_scene->get_root().add_child(torus.get_node());
  }

  void ObjectLayer::create_point(glm::vec3 position)
  {
    auto entity = m_scene->create_entity();

    entity->add_component<PointComponent>(entity->get_id());
    auto& point = entity->get_component<PointComponent>();

    auto [vertices, indices] = point.reconstruct();
    auto model               = std::make_shared<Model>(vertices,
                                         indices,
                                         std::vector<std::shared_ptr<EspTexture>>{},
                                         TorusInit::S_MODEL_PARAMS);
    entity->add_component<ModelComponent>(model, m_shader);

    point.get_node()->attach_entity(entity);
    point.get_node()->translate(position);

    m_scene->get_root().add_child(point.get_node());
  }

  void ObjectLayer::remove_object(ObjectInfo* info)
  {
    EspJob::done_all_jobs();
    m_scene->destroy_entity(info->m_id);
    ObjectRemovedEvent event{ info->m_name };
    post_event(event);
  }
} // namespace mg1