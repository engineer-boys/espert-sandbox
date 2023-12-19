#ifndef LAYERS_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_LAYER_HH

#include "ModelExampleUtils.hh"
#include "ModelExampleWithInstancingLayer.hh"
#include "ModelExampleWithoutInstancingLayer.hh"

// libs
#include "Espert.hh"

using namespace esp;
using namespace esp::action;

namespace model_example
{
  class ModelExampleLayer : public Layer
  {
   private:
    std::shared_ptr<Scene> m_scene;

    std::unique_ptr<Layer> m_model_example_with_instancing_layer;
    std::unique_ptr<Layer> m_model_example_without_instancing_layer;

    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

   public:
    ModelExampleLayer()
    {
      m_depth_block =
          EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

      m_final_product_plan = EspRenderPlan::build_final();
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });

      m_scene = Scene::create();
      m_scene->add_camera(std::make_shared<Camera>());
      auto camera = m_scene->get_camera(0);
      camera->set_position(glm::vec3{ 0.f, -1.f, -5.f });
      camera->look_at(glm::vec3{ 0.f, 0.f, 0.f });
      camera->set_move_speed(3.f);
      camera->set_sensitivity(4.f);
      m_scene->set_current_camera(camera.get());

      m_model_example_with_instancing_layer =
          std::make_unique<model_example_with_instancing::ModelExampleWithInstancingLayer>(m_scene);
      m_model_example_without_instancing_layer =
          std::make_unique<model_example_without_instancing::ModelExampleWithoutInstancingLayer>(m_scene);
    }

   private:
    virtual void update(float dt) override
    {
      m_final_product_plan->begin_plan();
      {
        auto camera = Scene::get_current_camera();
        camera->set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

        m_model_example_with_instancing_layer->update(dt);
        m_model_example_without_instancing_layer->update(dt);
      }
      m_final_product_plan->end_plan();
    }
  };
} // namespace model_example

#endif // LAYERS_MODEL_EXAMPLE_LAYER_HH
