#include <memory>

#include "CadLayer.hh"
#include "CoordinateSystemGridLayer.hh"
#include "CursorLayer.hh"
#include "GuiLayer.hh"
#include "ObjectLayer.hh"

namespace mg1
{
  CadLayer::CadLayer()
  {
    // create scene render plan [OFF-SCREEN]
    {
      m_scene_render.m_block = EspBlock::build(EspBlockFormat::ESP_FORMAT_R8G8B8A8_UNORM,
                                               EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT,
                                               { 0.1f, 0.1f, 0.2f });
      m_scene_render.m_depth_block =
          EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                               EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT,
                               EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
      m_scene_render.m_plan = EspRenderPlan::create();
      m_scene_render.m_plan->add_block(std::shared_ptr{ m_scene_render.m_block });
      m_scene_render.m_plan->add_depth_block(std::shared_ptr{ m_scene_render.m_depth_block });
      m_scene_render.m_plan->build();
    }

    // create final render plan [ON-SCREEN]
    {
      m_final_render.m_plan = EspRenderPlan::create_final();
      m_final_render.m_plan->build();

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      m_final_render.m_shader = ShaderSystem::acquire("Shaders/MG1/CadLayer/shader");
      m_final_render.m_shader->set_vertex_layouts(
          { VTX_LAYOUT(sizeof(QuadVertex),
                       0,
                       ESP_VERTEX_INPUT_RATE_VERTEX,
                       ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuadVertex, m_pos)),
                       ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuadVertex, m_tex_coord))) });
      m_final_render.m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_final_render.m_shader->build_worker();

      m_final_render.m_uniform_manager = m_final_render.m_shader->create_uniform_manager();
      m_final_render.m_uniform_manager->load_block(0, 0, m_scene_render.m_block.get());
      m_final_render.m_uniform_manager->build();

      m_final_render.m_vertex_buffer = EspVertexBuffer::create(quad.data(), sizeof(QuadVertex), quad.size());
      m_final_render.m_index_buffer  = EspIndexBuffer::create(quad_idx.data(), quad_idx.size());
    }

    // create scene
    {
      m_camera = std::make_shared<OrbitCamera>();
      m_scene  = Scene::create();
      m_scene->add_camera(m_camera);

      auto camera = m_scene->get_camera(0);
      camera->set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());
      m_scene->set_current_camera(camera.get());
    }

    // create children layers
    {
      m_children.emplace_back(new GuiLayer());
      m_children.emplace_back(new CursorLayer(m_scene.get()));
      m_children.emplace_back(new ObjectLayer(m_scene.get()));
      m_children.emplace_back(new CoordinateSystemGridLayer(m_scene.get()));
    }
  }

  void CadLayer::pre_update(float dt)
  {
    for (auto& child : m_children)
    {
      child->pre_update(dt);
    }
  }

  void CadLayer::update(float dt)
  {
    for (auto& child : m_children)
    {
      child->update(dt);
    }

    // scene render plan [OFF-SCREEN]
    m_scene_render.m_plan->begin_plan();
    {
      m_scene->draw();
    }
    m_scene_render.m_plan->end_plan();
    m_scene_render.m_depth_block->clear();

    // final render plan [ON-SCREEN]
    m_final_render.m_plan->begin_plan();
    {
      m_final_render.m_shader->attach();
      m_final_render.m_vertex_buffer->attach();
      m_final_render.m_index_buffer->attach();
      m_final_render.m_uniform_manager->attach();

      EspJob::draw_indexed(quad_idx.size());
      if (EspGui::m_use_gui) { EspGui::render(); }
    }
    m_final_render.m_plan->end_plan();
  }

  void CadLayer::post_update(float dt)
  {
    for (auto& child : m_children)
    {
      child->post_update(dt);
    }
  }

  void CadLayer::handle_event(esp::Event& event, float dt)
  {
    for (auto& child : m_children)
    {
      child->handle_event(event, dt);
    }

    Event::try_handler<MouseMovedEvent>(event, ESP_BIND_EVENT_FOR_FUN(CadLayer::mouse_moved_event_handler, dt));
    Event::try_handler<GuiSelectableChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(CadLayer::gui_selectable_changed_event_handler));
    Event::try_handler<GuiMouseStateChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(CadLayer::gui_mouse_state_changed_event_handler));
  }

  bool CadLayer::mouse_moved_event_handler(MouseMovedEvent& event, float dt)
  {
    if (!m_none_object_selected || !m_none_action_selected || !m_mouse_captured) { return false; }

    if (EspInput::is_mouse_button_pressed(ESP_MOUSE_BUTTON_LEFT))
    {
      m_camera->rotate(event.get_dx() * dt / 2, event.get_dy() * dt / 2);
    }
    if (EspInput::is_mouse_button_pressed(ESP_MOUSE_BUTTON_RIGHT)) { m_camera->zoom(event.get_dy() * dt / 2); }

    return true;
  }

  bool CadLayer::gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event)
  {
    if (event == GuiLabel::object_none) { m_none_object_selected = event.get_value(); }
    if (event == GuiLabel::action_none) { m_none_action_selected = event.get_value(); }

    return true;
  }

  bool CadLayer::gui_mouse_state_changed_event_handler(mg1::GuiMouseStateChangedEvent& event)
  {
    m_mouse_captured = !(bool)event.get_state();
    return false;
  }
} // namespace mg1