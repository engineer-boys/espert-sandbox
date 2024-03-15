#include <memory>

#include "CadCamLayer.hh"
#include "CoordinateSystemLayer.hh"
#include "CursorLayer.hh"
#include "Gui/GuiLayer.hh"
#include "ObjectLayer.hh"

namespace mg1
{
  CadCamLayer::CadCamLayer()
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
      m_children.emplace_back(new CursorLayer());
      m_children.emplace_back(new ObjectLayer(m_scene.get()));
      m_children.emplace_back(new CoordinateSystemLayer());
    }
  }

  void CadCamLayer::pre_update(float dt)
  {
    for (auto& child : m_children)
    {
      child->pre_update(dt);
    }
  }

  void CadCamLayer::update(float dt)
  {
    m_final_render_plan->begin_plan();
    {
      for (auto& child : m_children)
      {
        child->update(dt);
      }

      m_scene->draw();

      if (EspGui::m_use_gui) { EspGui::render(); }
    }
    m_final_render_plan->end_plan();
    m_depth_block->clear();
  }

  void CadCamLayer::post_update(float dt)
  {
    for (auto& child : m_children)
    {
      child->post_update(dt);
    }
  }

  void CadCamLayer::handle_event(esp::Event& event, float dt)
  {
    for (auto& child : m_children)
    {
      child->handle_event(event, dt);
    }

    Event::try_handler<MouseMovedEvent>(event, ESP_BIND_EVENT_FOR_FUN(CadCamLayer::mouse_moved_event_handler, dt));
    Event::try_handler<GuiListBoxChangedEvent>(event,
                                               ESP_BIND_EVENT_FOR_FUN(CadCamLayer::gui_list_box_changed_event_handler));
  }

  bool CadCamLayer::mouse_moved_event_handler(MouseMovedEvent& event, float dt)
  {
    if (!m_update_camera) { return false; }

    if (EspInput::is_mouse_button_pressed(ESP_MOUSE_BUTTON_LEFT))
    {
      m_camera->rotate(event.get_dx() * dt / 2, event.get_dy() * dt / 2);
    }
    if (EspInput::is_mouse_button_pressed(ESP_MOUSE_BUTTON_RIGHT)) { m_camera->zoom(event.get_dy() * dt / 2); }

    return true;
  }

  bool CadCamLayer::gui_list_box_changed_event_handler(GuiListBoxChangedEvent& event)
  {
    if (!event.label_equals(GuiFieldLabel::objects)) { return false; }

    bool none_selected = false;
    for (auto& id : event.get_value())
    {
      if (id == 0)
      {
        none_selected = true;
        break;
      }
    }

    if (none_selected) { m_update_camera = true; }
    else { m_update_camera = false; }

    return true;
  }
} // namespace mg1