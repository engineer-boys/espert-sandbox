#ifndef LAYERS_PBR_TEXTURE_LAYER_HH
#define LAYERS_PBR_TEXTURE_LAYER_HH

// libs
#include "../ModelExample/ModelExampleUtils.hh"
#include "Espert.hh"
#include "PBRUtils.hh"

using namespace esp;

namespace advance_rendering_example
{
  class PBRTextureLayer : public Layer
  {
   private:
    Camera m_camera{};
    std::shared_ptr<Model> m_model;

    std::vector<Light_PBR_Uniform> m_lights;

    std::unique_ptr<EspVertexBuffer> m_vertex_buffers_quad;
    std::unique_ptr<EspIndexBuffer> m_index_buffer_quad;

    struct
    {
      std::shared_ptr<EspDepthBlock> m_depth_block;
      std::shared_ptr<EspBlock> m_float_block;

      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspRenderPlan> m_product_plan;
    } m_first_pass;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
    } m_second_pass;

   public:
    PBRTextureLayer()
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
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(MVD_PBR_Uniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(MVP_PBR_Uniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE,
                                              sizeof(Light_PBR_Uniform),
                                              NUMBER_OF_LIGHTS);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_first_pass.m_shader = ShaderSystem::acquire("Shaders/PBRTextureExample/shader_p1");
        m_first_pass.m_shader->set_attachment_formats({ { EspBlockFormat::ESP_FORMAT_R32G32B32A32_SFLOAT } });
        m_first_pass.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                                 EspCompareOp::ESP_COMPARE_OP_LESS);
        m_first_pass.m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        m_first_pass.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_first_pass.m_shader->build_worker();

        m_first_pass.m_uniform_manager = m_first_pass.m_shader->create_uniform_manager();

        auto texture_albedo    = TextureSystem::acquire("Textures/RustedIron/albedo.png", {});
        auto texture_ao        = TextureSystem::acquire("Textures/RustedIron/ao.png", {});
        auto texture_metalic   = TextureSystem::acquire("Textures/RustedIron/metallic.png", {});
        auto texture_normal    = TextureSystem::acquire("Textures/RustedIron/normal.png", {});
        auto texture_roughness = TextureSystem::acquire("Textures/RustedIron/roughness.png", {});
        m_first_pass.m_uniform_manager->load_texture(0, 2, texture_albedo);
        m_first_pass.m_uniform_manager->load_texture(0, 3, texture_ao);
        m_first_pass.m_uniform_manager->load_texture(0, 4, texture_metalic);
        m_first_pass.m_uniform_manager->load_texture(0, 5, texture_normal);
        m_first_pass.m_uniform_manager->load_texture(0, 6, texture_roughness);
        m_first_pass.m_uniform_manager->build();

        Model::Builder model_builder{};
        model_builder.set_shader(m_first_pass.m_shader);
        model_builder.load_model("Models/sphere/sphere.gltf", { .layouts = {} });
        m_model = std::make_shared<Model>(model_builder);
      }

      // second pass
      {
        m_second_pass.m_final_product_plan = EspRenderPlan::build_final();

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_second_pass.m_shader = ShaderSystem::acquire("Shaders/PBRTextureExample/shader_p2");
        m_second_pass.m_shader->set_vertex_layouts(
            { VTX_LAYOUT(sizeof(QuatVertex_i),
                         0,
                         ESP_VERTEX_INPUT_RATE_VERTEX,
                         ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex_i, pos)),
                         ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex_i, texCoord))) });
        m_second_pass.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_second_pass.m_shader->build_worker();

        m_second_pass.m_uniform_manager = m_second_pass.m_shader->create_uniform_manager();
        m_second_pass.m_uniform_manager->load_block(0, 0, m_first_pass.m_float_block.get());
        m_second_pass.m_uniform_manager->build();

        m_vertex_buffers_quad = EspVertexBuffer::create(quad_i.data(), sizeof(QuatVertex_i), quad_i.size());
        m_index_buffer_quad   = EspIndexBuffer::create(quad_idx_i.data(), quad_idx_i.size());
      }

      m_lights = {};
      m_lights.push_back({ { 0.0f, 0.0f, 10.0f }, { 150.0f, 150.0f, 150.0f } });
      m_lights.push_back({ { 0.0f, 0.0f, 10.0f }, { 150.0f, 150.0f, 150.0f } });
      m_lights.push_back({ { 0.0f, 0.0f, 10.0f }, { 150.0f, 150.0f, 150.0f } });
      m_lights.push_back({ { 0.0f, 0.0f, 10.0f }, { 150.0f, 150.0f, 150.0f } });
      m_first_pass.m_uniform_manager->set_buffer_uniform(0,
                                                         1,
                                                         0,
                                                         m_lights.size() * sizeof(Light_PBR_Uniform),
                                                         m_lights.data());
    }

   private:
    virtual void update(float dt) override
    {
      const int nrRows    = 7;
      const int nrColumns = 7;
      const float spacing = 2.5;

      m_first_pass.m_product_plan->begin_plan();
      {
        Scene::set_current_camera(&m_camera);

        m_first_pass.m_shader->attach();

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
          for (int col = 0; col < nrColumns; ++col)
          {
            MVD_PBR_Uniform mvd{};
            mvd.moved = glm::vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, 0.0f);

            m_first_pass.m_uniform_manager->update_push_uniform(0, &mvd);
            m_model->draw();
          }
        }
      }
      m_first_pass.m_product_plan->end_plan();

      // .................................
      m_first_pass.m_depth_block->clear();
      // .................................

      m_second_pass.m_final_product_plan->begin_plan();
      {
        m_second_pass.m_shader->attach();
        m_vertex_buffers_quad->attach();

        m_second_pass.m_uniform_manager->attach();

        m_index_buffer_quad->attach();
        EspJob::draw_indexed(quad_idx_i.size());
      }
      m_second_pass.m_final_product_plan->end_plan();
    }
  };
} // namespace advance_rendering_example

#endif // LAYERS_PBR_TEXTURE_LAYER_HH
