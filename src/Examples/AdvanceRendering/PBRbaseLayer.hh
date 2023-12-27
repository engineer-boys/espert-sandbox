#ifndef LAYERS_PBR_BASE_LAYER_HH
#define LAYERS_PBR_BASE_LAYER_HH

// libs
#include "../ModelExample/ModelExampleUtils.hh"
#include "Espert.hh"

using namespace esp;

namespace advance_rendering_example
{
  const uint32_t NUMBER_OF_LIGHTS = 4;

  struct QuatVertex_i
  {
    glm::vec2 pos;
    glm::vec2 texCoord;

    QuatVertex_i(glm::vec2 pos, glm::vec2 texCoord) : pos{ pos }, texCoord{ texCoord } {}
  };

  static std::vector<QuatVertex_i> quad_i{ { { -1, -1 }, { 0, 0 } },
                                           { { 1, -1 }, { 1, 0 } },
                                           { { 1, 1 }, { 1, 1 } },
                                           { { -1, 1 }, { 0, 1 } } };

  static std::vector<uint32_t> quad_idx_i{ 0, 1, 2, 2, 3, 0 };

  struct MVP_PBR_Uniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    alignas(16) glm::vec3 camPos;
  };

  struct Material_PBR_Uniform
  {
    alignas(16) glm::vec3 moved;
    alignas(16) glm::vec3 albedo;
    alignas(4) float metallic;
    alignas(4) float roughness;
    alignas(4) float ao;
  };

  struct Light_PBR_Uniform
  {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;

    Light_PBR_Uniform(glm::vec3 position, glm::vec3 color) : position{ position }, color{ color } {}
  };

  class PBRBaseLayer : public Layer
  {
   private:
    Camera m_camera{};
    std::shared_ptr<Mesh> m_mesh;

    std::vector<Light_PBR_Uniform> m_lights;

    std::unique_ptr<EspVertexBuffer> m_vertex_buffers_quad;
    std::unique_ptr<EspIndexBuffer> m_index_buffer_quad;

    struct
    {
      std::shared_ptr<EspDepthBlock> m_depth_block;
      std::shared_ptr<EspBlock> m_float_block;

      std::unique_ptr<EspWorker> m_pipeline;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspRenderPlan> m_product_plan;
    } m_first_pass;

    struct
    {
      std::unique_ptr<EspWorker> m_pipeline;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
    } m_second_pass;

   public:
    PBRBaseLayer()
    {
      m_camera.set_position(glm::vec3(0.0f, -2.0f, 3.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      // first pass
      {
        m_first_pass.m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                                          EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

        m_first_pass.m_float_block = EspBlock::build(EspBlockFormat::ESP_FORMAT_R32G32B32A32_SFLOAT,
                                                     EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                                     { 0.2f, 0.3f, 0.4f });

        m_first_pass.m_product_plan = EspRenderPlan::build();
        m_first_pass.m_product_plan->add_block(std::shared_ptr{ m_first_pass.m_float_block });
        m_first_pass.m_product_plan->add_depth_block(std::shared_ptr{ m_first_pass.m_depth_block });

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_ALL_STAGES, 0, sizeof(Material_PBR_Uniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(MVP_PBR_Uniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE,
                                              sizeof(Light_PBR_Uniform),
                                              NUMBER_OF_LIGHTS);

        auto builder = EspWorkerBuilder::create();
        builder->set_attachment_formats({ { EspBlockFormat::ESP_FORMAT_R32G32B32A32_SFLOAT } });
        builder->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
        builder->set_shaders("../resources/Shaders/SphereExample/shader_p1.vert.spv",
                             "../resources/Shaders/SphereExample/shader_p1.frag.spv");
        builder->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        builder->set_pipeline_layout(std::move(uniform_meta_data));

        m_first_pass.m_pipeline = builder->build_worker();

        m_first_pass.m_uniform_manager = m_first_pass.m_pipeline->create_uniform_manager();
        m_first_pass.m_uniform_manager->build();

        m_mesh = std::shared_ptr<Mesh>{
          Model::Builder{}.load_model("Models/sphere/sphere.obj", Model::EspProcessFlipUVs).m_meshes[0]
        };
      }

      // second pass
      {
        m_second_pass.m_final_product_plan = EspRenderPlan::build_final();

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        auto builder = EspWorkerBuilder::create();
        builder->set_shaders("../resources/Shaders/SphereExample/shader_p2.vert.spv",
                             "../resources/Shaders/SphereExample/shader_p2.frag.spv");
        builder->set_vertex_layouts(
            { VTX_LAYOUT(sizeof(QuatVertex_i),
                         0,
                         ESP_VERTEX_INPUT_RATE_VERTEX,
                         ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex_i, pos)),
                         ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex_i, texCoord))) });
        builder->set_pipeline_layout(std::move(uniform_meta_data));
        m_second_pass.m_pipeline = builder->build_worker();

        m_second_pass.m_uniform_manager = m_second_pass.m_pipeline->create_uniform_manager();
        m_second_pass.m_uniform_manager->load_block(0, 0, m_first_pass.m_float_block.get());
        m_second_pass.m_uniform_manager->build();

        m_vertex_buffers_quad = EspVertexBuffer::create(quad_i.data(), sizeof(QuatVertex_i), quad_i.size());
        m_index_buffer_quad   = EspIndexBuffer::create(quad_idx_i.data(), quad_idx_i.size());
      }

      m_lights = {};
      m_lights.push_back({ { -30.0f, 30.0f, 30.0f }, { 300, 300, 300 } });
      m_lights.push_back({ { 30.0f, 30.0f, 30.0f }, { 300, 300, 300 } });
      m_lights.push_back({ { -30.0f, -30.0f, 30.0f }, { 300, 300, 300 } });
      m_lights.push_back({ { 30.0f, -30.0f, 30.0f }, { 300, 300, 300 } });
    }

   private:
    virtual void update(float dt) override
    {
      const int nrRows    = 7;
      const int nrColumns = 7;
      const float spacing = 5;

      m_first_pass.m_product_plan->begin_plan();
      {
        m_first_pass.m_uniform_manager->update_buffer_uniform(0,
                                                              1,
                                                              0,
                                                              m_lights.size() * sizeof(Light_PBR_Uniform),
                                                              m_lights.data());

        Scene::set_current_camera(&m_camera);

        m_first_pass.m_pipeline->attach();

        m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

        MVP_PBR_Uniform ubo{};
        ubo.model  = glm::mat4(1);
        ubo.view   = m_camera.get_view();
        ubo.proj   = m_camera.get_projection();
        ubo.camPos = m_camera.get_positiion();
        m_first_pass.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(MVP_PBR_Uniform), &ubo);
        m_first_pass.m_uniform_manager->attach();

        for (int row = 0; row < nrRows; ++row)
        {
          Material_PBR_Uniform material{};
          material.metallic = (float)row / (float)nrRows;
          for (int col = 0; col < nrColumns; ++col)
          {
            material.moved     = glm::vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, 0.0f);
            material.albedo    = glm::vec3(0.5f, 0.0f, 0.0f);
            material.roughness = glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f);
            material.ao        = 1.0f;

            m_first_pass.m_uniform_manager->update_push_uniform(0, &material);
            m_mesh->draw();
          }
        }
      }
      m_first_pass.m_product_plan->end_plan();

      // .................................
      m_first_pass.m_depth_block->clear();
      // .................................

      m_second_pass.m_final_product_plan->begin_plan();
      {
        m_second_pass.m_pipeline->attach();
        m_vertex_buffers_quad->attach();

        m_second_pass.m_uniform_manager->attach();

        m_index_buffer_quad->attach();
        EspJob::draw_indexed(quad_idx_i.size());
      }
      m_second_pass.m_final_product_plan->end_plan();
    }
  };
} // namespace advance_rendering_example

#endif // LAYERS_PBR_BASE_LAYER_HH
