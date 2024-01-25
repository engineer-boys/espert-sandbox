#ifndef LAYERS_ADVANCE_MODEL_LAYER_HH
#define LAYERS_ADVANCE_MODEL_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace advance_model
{

  struct PMVLV
  {
    glm::mat4 model;
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 lightPos = glm::vec4(5.0f, 5.0f, -5.0f, 1.0f);
    glm::vec4 viewPos;
  };

  class AdvanceModelLayer : public Layer
  {
   private:
    Camera m_camera{};
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Node> m_flight_helmet_node;

    struct
    {
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
    } m_final_pass;

   public:
    AdvanceModelLayer()
    {
      // final pass
      {
        m_final_pass.m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                                          EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

        m_final_pass.m_final_product_plan = EspRenderPlan::create_final({ 0.25f, 0.25f, 0.25f });
        m_final_pass.m_final_product_plan->add_depth_block(std::shared_ptr{ m_final_pass.m_depth_block });
        m_final_pass.m_final_product_plan->build();
      }

      // create shader
      ModelParams model_params = { .m_position                = true,
                                   .m_color                   = true,
                                   .m_normal                  = true,
                                   .m_tex_coord               = true,
                                   .m_bone_ids                = false,
                                   .m_weights                 = false,
                                   .m_tangent                 = false,
                                   .m_material_texture_layout = { { 1, 0, EspTextureType::ALBEDO } } };

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(PMVLV));
      uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(glm::mat4));
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto shader = ShaderSystem::acquire("Shaders/LoadAdvanceModel/shader");
      shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
      shader->set_vertex_layouts({ model_params.get_vertex_layouts() });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->build_worker();

      // create scene
      {
        m_scene              = Scene::create();
        m_flight_helmet_node = Node::create();
        m_scene->get_root().add_child(m_flight_helmet_node);

        m_camera.set_position(glm::vec3(0.0f, 0.0f, 1.0f));
        m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
        m_camera.set_move_speed(3.f);
        m_camera.set_sensitivity(4.f);

        auto model         = std::make_shared<Model>("AdvanceModels/FlightHelmet/FlightHelmet.gltf", model_params);
        auto flight_helmet = m_scene->create_entity("flight helmet");
        flight_helmet->add_component<ModelComponent>(model, shader);
        m_flight_helmet_node->attach_entity(flight_helmet);
      }
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_final_pass.m_final_product_plan->begin_plan();
      {
        // update
        m_flight_helmet_node->rotate(glm::radians(dt * 3), { 0, 1, 0 });

        PMVLV pmvlv{};
        pmvlv.model      = m_flight_helmet_node->get_model_mat();
        pmvlv.projection = m_camera.get_projection();
        pmvlv.view       = m_camera.get_view();
        pmvlv.viewPos    = glm::vec4(m_camera.get_position(), 1.0);

        auto& uniform_manager =
            m_flight_helmet_node->get_entity()->get_component<ModelComponent>().get_uniform_manager();
        uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(PMVLV), &pmvlv);

        // render
        m_scene->draw();
      }
      m_final_pass.m_final_product_plan->end_plan();

      m_final_pass.m_depth_block->clear();
    }
  };
} // namespace advance_model

#endif // LAYERS_ADVANCE_MODEL_LAYER_HH