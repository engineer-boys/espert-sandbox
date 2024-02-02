#ifndef LAYERS_PBR_BASIC_LAYER_HH
#define LAYERS_PBR_BASIC_LAYER_HH

// libs
#include "../ModelExample/ModelExampleUtils.hh"
#include "Espert.hh"
#include "PBR2Utils.hh"

using namespace esp;

namespace advance_rendering2_example
{
  class PBRBasicLayer : public Layer
  {
   private:
    struct MaterialPushBlock
    {
      float roughness;
      float metallic;
      float r, g, b;
    } m_params;

    struct UBOMatrices
    {
      glm::mat4 projection;
      glm::mat4 model;
      glm::mat4 view;
      glm::vec3 camPos;
    };

    struct
    {
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
      std::shared_ptr<EspBlock> m_float_block;

    } m_first_pass;

    struct
    {
      ModelParams m_model_params = { .m_position = true, .m_color = false, .m_normal = true };
      std::shared_ptr<Model> m_model;
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
    } m_sphere;

   private:
    Camera m_camera{};
    glm::vec3 m_lights[4];

    const uint32_t GRID_DIM = 7;

   public:
    PBRBasicLayer()
    {
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());
      m_camera.set_position(glm::vec3(10.0f, 13.0f, 1.8f));
      m_camera.look_at({ 10.0f, -1, 1.8f });
      m_camera.set_move_speed(4.0f);
      m_camera.set_sensitivity(1.0f);

      m_params.r         = 0.955008f;
      m_params.g         = 0.637427f;
      m_params.b         = 0.538163f;
      m_params.roughness = 0.1f;
      m_params.metallic  = 1.0f;

      // first pass
      {
        m_first_pass.m_depth_block =
            EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                 EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                 EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

        m_first_pass.m_final_product_plan = EspRenderPlan::create_final({ 0.025f, 0.025f, 0.025f });
        m_first_pass.m_final_product_plan->add_depth_block(std::shared_ptr{ m_first_pass.m_depth_block });
        m_first_pass.m_final_product_plan->build();

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(glm::vec3));
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_FRAG_STAGE,
                                            sizeof(glm::vec3),
                                            sizeof(MaterialPushBlock));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(UBOMatrices));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, 4 * sizeof(glm::vec3));

        m_sphere.m_shader = ShaderSystem::acquire("Shaders/AdvanceRendering2/PBRBasicExample/pbr");
        m_sphere.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspCompareOp::ESP_COMPARE_OP_LESS);
        m_sphere.m_shader->set_vertex_layouts({ m_sphere.m_model_params.get_vertex_layouts() });
        m_sphere.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_sphere.m_shader->build_worker();

        m_sphere.m_uniform_manager = m_sphere.m_shader->create_uniform_manager();
        m_sphere.m_uniform_manager->build();

        m_sphere.m_model = std::make_shared<Model>("Models/sphere/sphere.gltf", m_sphere.m_model_params);
      }

      // UPDATE LIGHTS
      {
        const float p = 15.0f;
        m_lights[0]   = glm::vec4(-p, -p * 0.5f, -p, 1.0f);
        m_lights[1]   = glm::vec4(-p, -p * 0.5f, p, 1.0f);
        m_lights[2]   = glm::vec4(p, -p * 0.5f, p, 1.0f);
        m_lights[3]   = glm::vec4(p, -p * 0.5f, -p, 1.0f);

        m_sphere.m_uniform_manager->set_buffer_uniform(0, 1, 0, 4 * sizeof(glm::vec3), m_lights);
      }
    }

   private:
    bool do_update = false;

    virtual void update(float dt) override
    {
      if (!do_update) return;

      Scene::set_current_camera(&m_camera);

      m_first_pass.m_final_product_plan->begin_plan();
      {
        m_sphere.m_shader->attach();
        UBOMatrices mvpc = {};

        mvpc.model      = glm::mat4(1);
        mvpc.view       = m_camera.get_view();
        mvpc.projection = m_camera.get_projection();
        mvpc.camPos     = m_camera.get_position();
        m_sphere.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(UBOMatrices), &mvpc);

        m_sphere.m_uniform_manager->attach();

        for (uint32_t y = 0; y < GRID_DIM; y++)
        {
          for (uint32_t x = 0; x < GRID_DIM; x++)
          {
            glm::vec3 pos = glm::vec3(float(x - (GRID_DIM / 2.0f)) * 2.5f, 0.0f, float(y - (GRID_DIM / 2.0f)) * 2.5f);
            m_sphere.m_uniform_manager->update_push_uniform(0, &pos);

            m_params.metallic  = glm::clamp((float)x / (float)(GRID_DIM - 1), 0.1f, 1.0f);
            m_params.roughness = glm::clamp((float)y / (float)(GRID_DIM - 1), 0.05f, 1.0f);
            m_sphere.m_uniform_manager->update_push_uniform(1, &m_params);

            for (auto node_info : *(m_sphere.m_model))
            {
              for (const auto& mesh_idx : node_info.m_current_node->m_meshes)
              {
                auto& mesh = m_sphere.m_model->m_meshes[mesh_idx];
                EspJob::draw_indexed(mesh.m_index_count, 1, mesh.m_first_index);
              }
            }
          }
        }
      }
      m_first_pass.m_final_product_plan->end_plan();
    }

    virtual void attach() override { do_update = true; }

    virtual void detach() override { do_update = false; }
  };
} // namespace advance_rendering2_example

#endif // LAYERS_PBR_BASIC_LAYER_HH
