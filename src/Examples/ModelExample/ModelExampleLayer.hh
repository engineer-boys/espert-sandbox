#ifndef LAYERS_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_LAYER_HH

#include "ModelExampleUtils.hh"

// libs
#include "Espert.hh"

using namespace esp;

namespace model_example
{
#define CUBES 5

  struct CubeUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };
  class ModelExampleLayer : public Layer
  {
   private:
    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

    ModelParams m_model_params = { .m_position = true, .m_color = true };

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Node> m_main_cube_node;

   public:
    ModelExampleLayer()
    {
      // create render plan
      m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                           EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                           EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

      m_final_product_plan = EspRenderPlan::create_final();
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });
      m_final_product_plan->build();

      // create shader
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(model_example::CameraPush));
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(CubeUniform));

      auto shader = ShaderSystem::acquire("Shaders/ModelExample/shader");
      shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      shader->set_vertex_layouts({ m_model_params.get_vertex_layouts() });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->build_worker();

      // create scene
      m_scene = Scene::create();
      m_scene->add_camera(std::make_shared<Camera>());
      auto camera = m_scene->get_camera(0);
      camera->set_position(glm::vec3{ 0.f, 1.f, 10.f });
      camera->look_at(glm::vec3{ 0.f, 0.f, 0.f });
      camera->set_move_speed(3.f);
      camera->set_sensitivity(4.f);
      m_scene->set_current_camera(camera.get());

      auto cube_model = std::make_shared<Model>(model_example::colored_cube,
                                                model_example::cube_idx,
                                                std::vector<std::shared_ptr<EspTexture>>{},
                                                m_model_params);

      std::array<std::shared_ptr<Entity>, CUBES> cubes{};
      for (auto& cube : cubes)
      {
        cube = m_scene->create_entity();
        cube->add_component<ModelComponent>(cube_model, shader);
      }

      m_main_cube_node = Node::create();
      m_scene->get_root().add_child(m_main_cube_node);
      m_main_cube_node->attach_entity(cubes[0]);
      m_main_cube_node->translate({ 0.f, .5f, -4.f });
      m_main_cube_node->scale(.5f);

      std::array<std::shared_ptr<Node>, CUBES> nodes{};
      nodes[0] = m_main_cube_node;
      for (int i = 1; i < CUBES; i++)
      {
        nodes[i] = Node::create();
        nodes[i - 1]->add_child(nodes[i]);
        nodes[i]->attach_entity(cubes[i]);
        nodes[i]->translate({ 1.5f / i, 0.f, 0.f });
        nodes[i]->scale(.5f);
      }
    }

   private:
    virtual void update(float dt) override
    {
      auto camera = Scene::get_current_camera();
      camera->set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_main_cube_node->rotate(-dt, { 0, 1, 0 });
      m_main_cube_node->set_translation(
          model_example::move_in_circle(m_main_cube_node->get_translation(), { 0, 1, 0 }, glm::radians(dt * 2)));

      m_main_cube_node->act(
          [&camera](Node* node)
          {
            CubeUniform ubo{};
            ubo.model = node->get_model_mat();
            ubo.view  = camera->get_view();
            ubo.proj  = camera->get_projection();

            auto& uniform_manager = node->get_entity()->get_component<ModelComponent>().get_uniform_manager();
            uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(CubeUniform), &ubo);
          });

      m_final_product_plan->begin_plan();
      {
        m_scene->draw();
      }
      m_final_product_plan->end_plan();
    }
  };

} // namespace model_example

#endif // LAYERS_MODEL_EXAMPLE_LAYER_HH
