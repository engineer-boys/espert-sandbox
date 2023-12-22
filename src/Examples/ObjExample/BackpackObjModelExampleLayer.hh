#ifndef LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace obj_example
{
  struct BackpackObjModelUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  class BackpackObjModelExampleLayer : public Layer
  {
   private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Node> m_backpack_node;
    Camera m_camera{};

    std::shared_ptr<Model> m_backpack_model;

    std::shared_ptr<EspShader> m_shader;

    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

   public:
    BackpackObjModelExampleLayer()
    {
      m_scene         = Scene::create();
      m_backpack_node = Node::create();
      m_scene->get_root().add_child(m_backpack_node);

      m_camera.set_position(glm::vec3(0.0f, -2.0f, 3.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(BackpackObjModelUniform));
      uniform_meta_data->establish_descriptor_set();
      // uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, 5);
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      m_depth_block =
          EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

      m_final_product_plan = EspRenderPlan::build_final();
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });

      m_shader = ShaderSystem::acquire("Shaders/ObjExample/BackpackObjModelExample/shader");
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
      m_shader->set_pipeline_layout(std::move(uniform_meta_data));
      m_shader->build_pipeline();

      Model::Builder model_builder{};
      model_builder.set_shader(m_shader);
      model_builder.load_model("Models/backpack/backpack.obj");
      m_backpack_model = std::make_shared<Model>(model_builder);

      auto backpack = m_scene->create_entity("backpack");
      backpack->add_component<TransformComponent>();
      backpack->add_component<ModelComponent>(m_backpack_model);

      m_backpack_node->attach_entity(backpack);
    }

   private:
    virtual void update(float dt) override
    {
      m_final_product_plan->begin_plan();
      {
        Scene::set_current_camera(&m_camera);
        m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

        BackpackObjModelUniform ubo{};
        ubo.model = glm::mat4{ 1.f };
        ubo.view  = m_camera.get_view();
        ubo.proj  = m_camera.get_projection();

        m_shader->attach();
        m_backpack_model->update_buffer_uniform(0, 0, 0, sizeof(BackpackObjModelUniform), &ubo);
        m_backpack_model->draw();
      }
      m_final_product_plan->end_plan();
    }
  };
} // namespace obj_example

#endif // LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH
