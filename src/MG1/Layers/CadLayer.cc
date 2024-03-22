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
    // create render plan
    {
      m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                           EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                           EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

      m_final_render_plan = EspRenderPlan::create_final();
      m_final_render_plan->add_depth_block(std::shared_ptr{ m_depth_block });
      m_final_render_plan->build();
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

    m_final_render_plan->begin_plan();
    {
      if (EspGui::m_use_gui) { EspGui::render(); }
      m_scene->draw();
    }
    m_final_render_plan->end_plan();
    m_depth_block->clear();
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
    if (!m_update_camera || !m_update_mouse) { return false; }

    if (EspInput::is_mouse_button_pressed(ESP_MOUSE_BUTTON_LEFT))
    {
      m_camera->rotate(event.get_dx() * dt / 2, event.get_dy() * dt / 2);
    }
    if (EspInput::is_mouse_button_pressed(ESP_MOUSE_BUTTON_RIGHT)) { m_camera->zoom(event.get_dy() * dt / 2); }

    return true;
  }

  bool CadLayer::gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event)
  {
    if (event == GuiLabel::object_none) { m_update_camera = event.get_value(); }
    if (event == GuiLabel::action_set_cursor_pos) { m_update_camera = !event.get_value(); }

    return true;
  }

  bool CadLayer::gui_mouse_state_changed_event_handler(mg1::GuiMouseStateChangedEvent& event)
  {
    m_update_mouse = !(bool)event.get_state();
    return false;
  }
} // namespace mg1