#ifndef LAYERS_EXAMPLE_INSTANCING_LAYER_H
#define LAYERS_EXAMPLE_INSTANCING_LAYER_H

// libs
#include "Espert.hh"

using namespace esp;

namespace my_game
{
  struct InstancingExampleVertex
  {
    glm::vec2 position;
    glm::vec3 color;
  };

  struct InstancingExampleUniform
  {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  };

  static InstancingExampleUniform get_new_instancing_example_uniform()
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    InstancingExampleUniform mvp{};
    mvp.model = glm::scale(glm::mat4(1.0f), { 0.05f, 0.05f, 0.05f });
    mvp.model = glm::rotate(mvp.model, time * glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    mvp.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.25f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    mvp.proj =
        glm::perspective(glm::radians(45.0f), EspFrameManager::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

    return mvp;
  }

  class InstancingExampleLayer : public esp::Layer
  {
    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::vector<ExampleVertex> m_square    = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                               { { -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                               { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                               { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } } };
    std::vector<uint32_t> m_square_indices = { 0, 2, 1, 2, 0, 3 };
    std::vector<glm::vec2> m_instance_pos{};

    std::unique_ptr<EspVertexBuffers> m_vertex_buffers;
    std::unique_ptr<EspIndexBuffer> m_square_index_buffer;

   public:
    InstancingExampleLayer()
    {
      for (float x = -1.f; x <= 1.f; x += .25f)
      {
        for (float y = -1.f; y <= 1.f; y += .25f)
        {
          m_instance_pos.emplace_back(x, y);
        }
      }

      auto pp_layout = EspUniformMetaData::create();
      pp_layout->establish_descriptor_set();
      pp_layout->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(MVP));

      auto builder = EspPipelineBuilder::create();
      builder->set_shaders("../resources/Shaders/InstancingExample/shader.vert.spv",
                           "../resources/Shaders/InstancingExample/shader.frag.spv");
      builder->set_vertex_layouts({
          VTX_LAYOUT(sizeof(ExampleVertex),
                     0,
                     ESP_VERTEX_INPUT_RATE_VERTEX,
                     ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(ExampleVertex, position)),
                     ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(ExampleVertex, color))),
          VTX_LAYOUT(sizeof(glm::vec2),
                     1,
                     ESP_VERTEX_INPUT_RATE_INSTANCE,
                     ATTR(2, ESP_FORMAT_R32G32_SFLOAT, 0)) /* VTX_LAYOUT*/
      }                                                    /* VTX_LAYOUTS */
      );
      builder->set_pipeline_layout(std::move(pp_layout));
      m_pipeline        = builder->build_pipeline();
      m_uniform_manager = m_pipeline->create_uniform_manager();
      m_uniform_manager->build();

      m_vertex_buffers = EspVertexBuffers::create();
      m_vertex_buffers->add(m_square.data(), sizeof(ExampleVertex), m_square.size());
      m_vertex_buffers->add(m_instance_pos.data(), sizeof(m_instance_pos[0]), m_instance_pos.size());

      m_square_index_buffer = EspIndexBuffer::create(m_square_indices.data(), m_square_indices.size());
    }

   private:
    virtual ~InstancingExampleLayer() override {}

    virtual void attach() override {}

    virtual void detach() override {}

    virtual void update(float dt) override
    {
      m_pipeline->attach();
      m_vertex_buffers->attach();
      m_square_index_buffer->attach();

      auto mvp = get_new_instancing_example_uniform();
      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(MVP), &mvp);
      m_uniform_manager->attach();

      m_square_index_buffer->attach();
      EspCommandHandler::draw_indexed(m_square_indices.size(), m_instance_pos.size());
    }
  };
} // namespace my_game

#endif // LAYERS_EXAMPLE_INSTANCING_LAYER_H
