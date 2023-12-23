#ifndef LAYERS_OFFSCREEN_RENDERING_LAYER_HH
#define LAYERS_OFFSCREEN_RENDERING_LAYER_HH

// libs
#include "Espert.hh"
#include "ModelExample/ModelExampleUtils.hh"

using namespace esp;

namespace advance_rendering_example
{
  struct QuatVertex
  {
    glm::vec2 pos;
    glm::vec2 texCoord;

    QuatVertex(glm::vec2 pos, glm::vec2 texCoord) : pos{ pos }, texCoord{ texCoord } {}
  };

  static std::vector<QuatVertex> quad{ { { -1, -1 }, { 0, 0 } },
                                       { { 1, -1 }, { 1, 0 } },
                                       { { 1, 1 }, { 1, 1 } },
                                       { { -1, 1 }, { 0, 1 } } };

  static std::vector<uint32_t> quad_idx{ 0, 1, 2, 2, 3, 0 };

  struct MVPExampleUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  static MVPExampleUniform get_new_MVP()
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    MVPExampleUniform ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj =
        glm::perspective(glm::radians(45.0f), EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

    return ubo;
  }

  class OffscreenRenderingLayer : public Layer
  {
    std::shared_ptr<EspShader> m_shader_off;
    std::shared_ptr<EspShader> m_shader_on;
    std::unique_ptr<EspUniformManager> m_uniform_manager_off;
    std::unique_ptr<EspUniformManager> m_uniform_manager_on;

    std::vector<Mesh::Vertex> m_vertices = model_example::create_cube_vertices();
    std::unique_ptr<EspVertexBuffer> m_vertex_buffers;

    std::unique_ptr<EspVertexBuffer> m_vertex_buffers_quad;
    std::unique_ptr<EspIndexBuffer> m_index_buffer_quad;

    std::shared_ptr<EspBlock> m_block;
    std::shared_ptr<EspDepthBlock> m_depth_block;

    std::unique_ptr<EspRenderPlan> m_product_plan;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

   public:
    OffscreenRenderingLayer()
    {
      // 1. render pass [OFF-SCREEN]
      {
        m_block = EspBlock::build(EspBlockFormat::ESP_FORMAT_B8G8R8A8_SRGB,
                                  EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT,
                                  { 0.2f, 0.3f, 0.4f });

        m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT);

        m_product_plan = EspRenderPlan::build();
        m_product_plan->add_block(std::shared_ptr{ m_block });
        m_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(MVPExampleUniform));

        m_shader_off = ShaderSystem::acquire("Shaders/OffscreenRnd/shader");
        m_shader_off->enable_multisampling(EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT);
        m_shader_off->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
        m_shader_off->set_vertex_layouts(
            { VTX_LAYOUT(sizeof(Mesh::Vertex),
                         0,
                         ESP_VERTEX_INPUT_RATE_VERTEX,
                         ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::Vertex, m_position)),
                         ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::Vertex, m_color)),
                         ATTR(2, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::Vertex, m_normal)),
                         ATTR(3, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(Mesh::Vertex, m_tex_coord))) });
        m_shader_off->set_worker_layout(std::move(uniform_meta_data));
        m_shader_off->build_worker();

        m_uniform_manager_off = m_shader_off->create_uniform_manager();
        m_uniform_manager_off->build();

        m_vertex_buffers = EspVertexBuffer::create(m_vertices.data(), sizeof(Mesh::Vertex), m_vertices.size());

        m_index_buffer_quad = EspIndexBuffer::create(quad_idx.data(), quad_idx.size());
      }

      // 2. render pass [ON-SCREEN]
      {
        m_final_product_plan = EspRenderPlan::build_final();

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        // uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(MVPExampleUniform));

        m_shader_on = ShaderSystem::acquire("Shaders/OffscreenRnd/shader_on");
        m_shader_on->set_vertex_layouts(
            { VTX_LAYOUT(sizeof(QuatVertex),
                         0,
                         ESP_VERTEX_INPUT_RATE_VERTEX,
                         ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex, pos)),
                         ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex, texCoord))) });
        m_shader_on->set_worker_layout(std::move(uniform_meta_data));
        m_shader_on->build_worker();

        m_uniform_manager_on = m_shader_on->create_uniform_manager();
        m_uniform_manager_on->load_block(0, 0, m_block.get());
        m_uniform_manager_on->build();

        m_vertex_buffers_quad = EspVertexBuffer::create(quad.data(), sizeof(QuatVertex), quad.size());
      }

      ESP_INFO("AFTER CREATE ALL IN LAYER ...");
    }

   private:
    virtual ~OffscreenRenderingLayer() override {}

    virtual void update(float dt) override
    {
      // 1. render pass
      m_product_plan->begin_plan();
      {
        m_shader_off->attach();
        m_vertex_buffers->attach();

        auto ubo = get_new_MVP();
        m_uniform_manager_off->update_buffer_uniform(0, 0, 0, sizeof(MVPExampleUniform), &ubo);
        m_uniform_manager_off->attach();

        EspJob::draw(m_vertices.size());
      }
      m_product_plan->end_plan();

      m_depth_block->clear();

      // 2. render pass
      m_final_product_plan->begin_plan();
      {
        m_shader_on->attach();
        m_vertex_buffers_quad->attach();

        m_uniform_manager_on->attach();

        m_index_buffer_quad->attach();
        EspJob::draw_indexed(quad_idx.size());
      }
      m_final_product_plan->end_plan();
    }
  };
} // namespace advance_rendering_example

#endif // LAYERS_OFFSCREEN_RENDERING_LAYER_HH
