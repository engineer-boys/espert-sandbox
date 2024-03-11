#include "CadCamLayer.hh"
#include "CursorLayer.hh"
#include "Gui/GuiLayer.hh"
#include "TorusLayer.hh"

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
      m_scene = Scene::create();
      m_scene->add_camera(std::make_shared<Camera>());

      auto camera = m_scene->get_camera(0);
      //      camera->set_position(glm::vec3{ 0.f, 1.f, 5.f });
      //      camera->look_at(glm::vec3{ 0.f, 0.f, 0.f });
      camera->set_move_speed(3.f);
      camera->set_sensitivity(4.f);
      camera->set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());
      m_scene->set_current_camera(camera.get());
    }

    // create children layers
    {
      m_children.emplace_back(new GuiLayer());
      m_children.emplace_back(new CursorLayer());
      m_children.emplace_back(new TorusLayer(m_scene.get()));
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
  }
} // namespace mg1