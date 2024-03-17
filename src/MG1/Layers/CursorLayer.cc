#include "CursorLayer.hh"

#define CURSOR_SIZE  0.025f
#define CURSOR_COLOR glm::vec3(1, 1, 1)

namespace mg1
{
  CursorLayer::CursorLayer()
  {
    // create shader
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(CursorParams));
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::vec2));

      m_shader = ShaderSystem::acquire("Shaders/MG1/CursorLayer/shader");
      m_shader->set_input_assembly_settings({ .m_primitive_topology = ESP_PRIMITIVE_TOPOLOGY_POINT_LIST });
      // m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->set_rasterizer_settings({ .m_line_width = 3.f });
      m_shader->build_worker();

      m_uniform_manager = m_shader->create_uniform_manager(0, 1);
      m_uniform_manager->build();

      //        CursorParams ubo{ CURSOR_SIZE, CURSOR_COLOR, EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio()
      //        }; m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(CursorParams), &ubo);
    }
  }

  void CursorLayer::update(float dt)
  {
    if (!m_update) return;

    m_shader->attach();

    // TODO: this shouldn't have to be updated
    CursorParams ubo{ CURSOR_SIZE, CURSOR_COLOR, EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio() };
    m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(CursorParams), &ubo);
    //

    glm::vec2 mouse_pos{ EspInput::get_mouse_x() / EspWindow::get_width() * 2.f - 1.f,
                         1.f - EspInput::get_mouse_y() / EspWindow::get_height() * 2.f };
    m_uniform_manager->update_buffer_uniform(1, 0, 0, sizeof(glm::vec2), &mouse_pos);

    m_uniform_manager->attach();

    EspJob::draw(1);
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
} // namespace mg1