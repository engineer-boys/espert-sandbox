#ifndef LAYERS_TEXTURE_EXAMPLE_LAYER_HH
#define LAYERS_TEXTURE_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace my_game
{
  struct TextureExampleVertex
  {
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 tex_coord;
  };

  struct TextureExampleUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  static TextureExampleUniform get_new_texture_example_uniform()
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    TextureExampleUniform ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj =
        glm::perspective(glm::radians(45.0f), EspFrameManager::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    return ubo;
  }

  class TextureExampleLayer : public Layer
  {
    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::vector<TextureExampleVertex> m_vertices = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
                                                     { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
                                                     { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
                                                     { { -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } } };
    std::vector<uint32_t> m_indices              = { 0, 1, 2, 2, 3, 0 };

    std::unique_ptr<EspVertexBuffers> m_vertex_buffers;
    std::unique_ptr<EspIndexBuffer> m_index_buffer;

   public:
    TextureExampleLayer()
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(TextureExampleUniform));
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto builder = EspPipelineBuilder::create();
      builder->set_shaders("../shaders/TextureExample/shader.vert.spv", "../shaders/TextureExample/shader.frag.spv");
      builder->set_vertex_layouts(
          { VTX_LAYOUT(sizeof(TextureExampleVertex),
                       0,
                       ESP_VERTEX_INPUT_RATE_VERTEX,
                       ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(TextureExampleVertex, position)),
                       ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(TextureExampleVertex, color)),
                       ATTR(2, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(TextureExampleVertex, tex_coord))) });
      builder->set_pipeline_layout(std::move(uniform_meta_data));
      m_pipeline = builder->build_pipeline();

      m_uniform_manager = m_pipeline->create_uniform_manager();

      m_uniform_manager->load_texture(0, 1, "../textures/image.jpeg");

      m_uniform_manager->build();

      m_vertex_buffers = EspVertexBuffers::create();
      m_vertex_buffers->add(m_vertices.data(), sizeof(TextureExampleVertex), m_vertices.size());

      m_index_buffer = EspIndexBuffer::create(m_indices.data(), m_indices.size());
    }

   private:
    virtual ~TextureExampleLayer() override {}

    virtual void update() override
    {
      m_pipeline->attach();
      m_vertex_buffers->attach();
      m_index_buffer->attach();

      auto ubo = get_new_texture_example_uniform();
      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(TextureExampleUniform), &ubo);
      m_uniform_manager->attach();

      m_index_buffer->attach();
      EspCommandHandler::draw_indexed(m_indices.size());
    }
  };
} // namespace my_game

#endif // LAYERS_TEXTURE_EXAMPLE_LAYER_HH