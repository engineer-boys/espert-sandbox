#ifndef LAYERS_TEXTURE_DEPTH_EXAMPLE_LAYER_HH
#define LAYERS_TEXTURE_DEPTH_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace my_game
{
  struct TextureDepthExampleVertex
  {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 tex_coord;
  };

  struct TextureDepthExampleUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  static TextureDepthExampleUniform get_new_texture_depth_example_uniform()
  {
    TextureDepthExampleUniform ubo{};
    ubo.model = glm::mat4(1.f);
    ubo.view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj =
        glm::perspective(glm::radians(45.0f), EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

    return ubo;
  }

  class TextureDepthExampleLayer : public Layer
  {
    std::shared_ptr<EspShader> m_shader;
    std::shared_ptr<Material> m_material;

    std::vector<TextureDepthExampleVertex> m_vertices = {
      { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
      { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
      { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
      { { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

      { { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
      { { 0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
      { { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
      { { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
    };
    std::vector<uint32_t> m_indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };

    std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    std::unique_ptr<EspIndexBuffer> m_index_buffer;

    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

   public:
    TextureDepthExampleLayer()
    {
      m_depth_block =
          EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

      m_final_product_plan = EspRenderPlan::build_final();
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(TextureDepthExampleUniform));
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      m_shader = ShaderSystem::acquire("Shaders/TextureDepthExample/shader");
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({ VTX_LAYOUT(
          sizeof(TextureDepthExampleVertex),
          0,
          ESP_VERTEX_INPUT_RATE_VERTEX,
          ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(TextureDepthExampleVertex, position)),
          ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(TextureDepthExampleVertex, color)),
          ATTR(2, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(TextureDepthExampleVertex, tex_coord))) });
      m_shader->set_pipeline_layout(std::move(uniform_meta_data));
      m_shader->build_pipeline();

      auto texture = TextureSystem::acquire("Textures/image.jpeg");
      m_material =
          MaterialSystem::acquire("david_material", { texture }, m_shader, { { 0, 1, EspTextureType::ALBEDO } });

      m_vertex_buffer =
          EspVertexBuffer::create(m_vertices.data(), sizeof(TextureDepthExampleVertex), m_vertices.size());

      m_index_buffer = EspIndexBuffer::create(m_indices.data(), m_indices.size());
    }

   private:
    virtual ~TextureDepthExampleLayer() override {}

    virtual void update(float dt) override
    {
      m_final_product_plan->begin_plan();
      {
        m_shader->attach();
        m_vertex_buffer->attach();
        m_index_buffer->attach();

        auto ubo = get_new_texture_depth_example_uniform();
        m_material->update_buffer_uniform(0, 0, 0, sizeof(TextureDepthExampleUniform), &ubo);
        m_material->attach();

        EspJob::draw_indexed(m_indices.size());
      }
      m_final_product_plan->end_plan();
    }
  };
} // namespace my_game

#endif // LAYERS_TEXTURE_DEPTH_EXAMPLE_LAYER_HH