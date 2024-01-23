#ifndef LAYERS_ADVANCE_MODEL_LAYER_HH
#define LAYERS_ADVANCE_MODEL_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace advance_model
{

  struct PMVLV
  {
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 lightPos = glm::vec4(5.0f, 5.0f, -5.0f, 1.0f);
    glm::vec4 viewPos;
  };

  class AdvanceModelLayer : public Layer
  {
   private:
    Camera m_camera{};
    NModelParams m_params;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_managers;
      std::unordered_map<std::shared_ptr<Material>, std::unique_ptr<EspUniformManager>> m_material_uniform_managers;
      std::unique_ptr<NModel> m_model;
    } m_sphere;

    struct
    {
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
    } m_final_pass;

   public:
    AdvanceModelLayer()
    {
      m_camera.set_position(glm::vec3(0.0f, 0.0f, 2.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
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
                     .m_bone_ids        = false,
                     .m_weights         = false,
                     .m_tangent         = false,
                     .m_material_texture_layout = {{1, 0, EspTextureType::ALBEDO}} };

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(PMVLV));
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(glm::mat4));
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_sphere.m_shader = ShaderSystem::acquire("Shaders/LoadAdvanceModel/shader");
        m_sphere.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
        m_sphere.m_shader->set_vertex_layouts({ m_params.get_vertex_layouts() });
        m_sphere.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_sphere.m_shader->build_worker();
        m_sphere.m_uniform_managers = m_sphere.m_shader->create_uniform_manager(0, 0);
        m_sphere.m_uniform_managers->build();

        m_sphere.m_model = std::make_unique<NModel>("AdvanceModels/FlightHelmet/FlightHelmet.gltf", m_params);
        for (const auto& mesh : m_sphere.m_model->m_meshes) {
          if (!m_sphere.m_material_uniform_managers.contains(mesh.m_material)) m_sphere.m_material_uniform_managers.insert({mesh.m_material, mesh.m_material->create_uniform_manager(m_sphere.m_shader)});
        }

        // Example of changing position of model
        // auto rotation = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // m_sphere.m_model->set_localisation(rotation);
      }
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_final_pass.m_final_product_plan->begin_plan();
      {
        m_sphere.m_shader->attach();

        PMVLV pmvlv{};
        pmvlv.projection = m_camera.get_projection();
        pmvlv.view       = m_camera.get_view();

        pmvlv.viewPos = glm::vec4(m_camera.get_positiion(), 1.0);
        m_sphere.m_uniform_managers->update_buffer_uniform(0, 0, 0, sizeof(PMVLV), &pmvlv);
        m_sphere.m_uniform_managers->attach();

        for (auto node : *(m_sphere.m_model))
        {
          m_sphere.m_uniform_managers->update_push_uniform(0, &(node.m_current_node->m_precomputed_transformation));

          for (auto& mesh_idx : node.m_current_node->m_meshes)
          {
            auto& mesh = m_sphere.m_model->m_meshes[mesh_idx];

            m_sphere.m_material_uniform_managers.at(mesh.m_material)->attach();
            EspJob::draw_indexed(mesh.m_index_count, 1, mesh.m_first_index);
          }
        }
      }
      m_final_pass.m_final_product_plan->end_plan();

      m_final_pass.m_depth_block->clear();
    }
  };
} // namespace advance_model

#endif // LAYERS_ADVANCE_MODEL_LAYER_HH
