#ifndef LAYERS_ANIMATED_MODEL_LAYER_HH
#define LAYERS_ANIMATED_MODEL_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace advance_model
{

  struct PMVLV2
  {
    glm::mat4 view;
    glm::mat4 projection;
  };

  class AnimatedModelLayer : public Layer
  {
   private:
    std::shared_ptr<Scene> m_scene;
    Camera m_camera{};
    std::shared_ptr<Animator> m_animator;

    struct
    {
      std::shared_ptr<Node> m_node;
      std::shared_ptr<Model> m_model;
    } m_cesium_man;

    struct
    {
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
    } m_final_pass;

   public:
    AnimatedModelLayer()
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
                                   .m_bone_ids                = true,
                                   .m_weights                 = true,
                                   .m_tangent                 = false,
                                   .m_material_texture_layout = { { 1, 0, EspTextureType::ALBEDO } } };

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(PMVLV2));
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 100 * sizeof(glm::mat4));
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto shader = ShaderSystem::acquire("Shaders/AnimatedModel/shader");
      shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
      shader->set_vertex_layouts({ model_params.get_vertex_layouts() });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->build_worker();

      // create scene
      {
        m_scene             = Scene::create();
        m_cesium_man.m_node = Node::create();
        m_scene->get_root().add_child(m_cesium_man.m_node);

        m_camera.set_position(glm::vec3(0.0f, 1.0f, 2.0f));
        m_camera.look_at(glm::vec3(0.0f, 1.2f, 0.0f));
        m_camera.set_move_speed(3.f);
        m_camera.set_sensitivity(4.f);

        m_cesium_man.m_model = std::make_shared<Model>("AdvanceModels/CesiumMan/CesiumMan.gltf", model_params);
        auto cesium_man      = m_scene->create_entity("cesium man");
        cesium_man->add_component<ModelComponent>(m_cesium_man.m_model, shader);
        m_cesium_man.m_node->attach_entity(cesium_man);

        m_animator = std::make_shared<Animator>();
        m_animator->play_animation(m_cesium_man.m_model.get(), 0, AnimationPeriodType::NON_STOP);
      }
    }

   private:
    virtual void update(float dt) override
    {
      m_animator->update_animation(dt, m_cesium_man.m_model.get()); // TODO: pass model by reference (?)

      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_final_pass.m_final_product_plan->begin_plan();
      {
        // update
        PMVLV2 pmvlv{};
        pmvlv.projection = m_camera.get_projection();
        pmvlv.view       = m_camera.get_view();

        auto& uniform_manager =
            m_cesium_man.m_node->get_entity()->get_component<ModelComponent>().get_uniform_manager();
        uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(PMVLV2), &pmvlv);

        auto matrices = m_animator->get_final_bone_matrices();
        uniform_manager.update_buffer_uniform(0, 1, 0, 100 * sizeof(glm::mat4), matrices);

        // render
        m_scene->draw();
      }
      m_final_pass.m_final_product_plan->end_plan();

      m_final_pass.m_depth_block->clear();
    }
  };
} // namespace advance_model

#endif // LAYERS_ANIMATED_MODEL_LAYER_HH
