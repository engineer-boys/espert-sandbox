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
    Camera m_camera{};

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_managers;
      std::unique_ptr<EspUniformManager> m_uniform_managers_material;
      std::unique_ptr<NModel> m_model;
    } m_sphere;

    struct
    {
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
    } m_final_pass;

    std::shared_ptr<Animator> m_animator;
    NModelParams m_params;

   public:
    AnimatedModelLayer()
    {
      m_camera.set_position(glm::vec3(0.0f, 1.0f, 2.0f));
      m_camera.look_at(glm::vec3(0.0f, 1.2f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      // final pass
      {
        m_final_pass.m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                                          EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

        m_final_pass.m_final_product_plan = EspRenderPlan::create_final({ 0.25f, 0.25f, 0.25f });
        m_final_pass.m_final_product_plan->add_depth_block(std::shared_ptr{ m_final_pass.m_depth_block });
        m_final_pass.m_final_product_plan->build();
      }

      // model
      {
        m_params = { .m_position        = true,
                     .m_color           = true,
                     .m_normal          = true,
                     .m_tex_coord       = true,
                     .m_bone_ids        = true,
                     .m_weights         = true,
                     .m_tangent         = false,
                     .m_material_params = { .m_albedo = true } };

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(PMVLV2));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 100 * sizeof(glm::mat4));
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_sphere.m_shader = ShaderSystem::acquire("Shaders/AnimatedModel/shader");
        m_sphere.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
        m_sphere.m_shader->set_vertex_layouts({ m_params.get_vertex_layouts() });
        m_sphere.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_sphere.m_shader->build_worker();

        {
          m_sphere.m_uniform_managers = m_sphere.m_shader->create_uniform_manager(0, 0);
          m_sphere.m_uniform_managers->build();
        }

        m_sphere.m_model = std::make_unique<NModel>("AdvanceModels/CesiumMan/CesiumMan.gltf", m_params);
        {
          m_sphere.m_uniform_managers_material = m_sphere.m_shader->create_uniform_manager(1, 1);

          auto& material = m_sphere.m_model->m_materials[0];
          auto& texture  = m_sphere.m_model->m_textures[material.m_albedo.value()];

          m_sphere.m_uniform_managers_material->load_texture(1, 0, texture);
          m_sphere.m_uniform_managers_material->build();
        }

        m_animator = std::shared_ptr<Animator>{ new Animator() };
        m_animator->play_animation(m_sphere.m_model.get(), 0, AnimationPeriodType::MANY_TIMES, 3);
      }
    }

   private:
    virtual void update(float dt) override
    {
      m_animator->update_animation(dt, m_sphere.m_model.get());

      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_final_pass.m_final_product_plan->begin_plan();
      {
        m_sphere.m_shader->attach();

        PMVLV2 pmvlv{};
        pmvlv.projection = m_camera.get_projection();
        pmvlv.view       = m_camera.get_view();

        m_sphere.m_uniform_managers->update_buffer_uniform(0, 0, 0, sizeof(PMVLV2), &pmvlv);

        auto matrices = m_animator->get_final_bone_matrices();
        m_sphere.m_uniform_managers->update_buffer_uniform(0, 1, 0, 100 * sizeof(glm::mat4), matrices);
        m_sphere.m_uniform_managers->attach();

        m_sphere.m_uniform_managers_material->attach();

        for (auto node : *(m_sphere.m_model))
        {
          for (auto& mesh_idx : node.m_current_node->m_meshes)
          {
            EspJob::draw_indexed(m_sphere.m_model->m_meshes[mesh_idx].m_index_count,
                                 1,
                                 m_sphere.m_model->m_meshes[mesh_idx].m_first_index);
          }
        }
      }
      m_final_pass.m_final_product_plan->end_plan();

      m_final_pass.m_depth_block->clear();
    }
  };
} // namespace advance_model

#endif // LAYERS_ANIMATED_MODEL_LAYER_HH
