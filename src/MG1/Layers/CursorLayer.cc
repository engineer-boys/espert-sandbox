#include "CursorLayer.hh"
#include "MG1/Common/InitInfo.hh"

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
      m_shader->set_rasterizer_settings(
          { .m_polygon_mode = ESP_POLYGON_MODE_LINE, .m_cull_mode = ESP_CULL_MODE_NONE, .m_line_width = 5.f });
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({ CursorInit::S_MODEL_PARAMS.get_vertex_layouts() });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->build_worker();
    }

    create_cursor(CursorType::Mouse);
  }

  void CursorLayer::update(float dt)
  {
    if (!m_update) return;

    auto camera  = Scene::get_current_camera();
    glm::mat4 vp = camera->get_projection() * camera->get_view();

    auto view = m_scene->m_registry.view<CursorComponent, ModelComponent>();
    for (auto&& [entity, cursor, model] : view.each())
    {
      cursor.update();

      auto& uniform_manager = model.get_uniform_manager();
      glm::vec3 cursor_pos  = cursor.get_position();
      uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(glm::vec3), &cursor_pos);
      uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(glm::mat4), &vp);
    }
  }

  void CursorLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<GuiMouseStateChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(CursorLayer::gui_mouse_state_changed_event_handler));
  }

  bool CursorLayer::gui_mouse_state_changed_event_handler(mg1::GuiMouseStateChangedEvent& event)
  {
    m_update = !(bool)event.get_state();
    return false;
  }

  void CursorLayer::create_cursor(mg1::CursorType type, glm::vec3 position)
  {
    auto entity = m_scene->create_entity();

    entity->add_component<CursorComponent>(entity->get_id(), type);
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
} // namespace mg1