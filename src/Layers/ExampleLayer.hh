#ifndef LAYERS_EXAMPLE_LAYER_H
#define LAYERS_EXAMPLE_LAYER_H

// libs
#include "Espert.hh"

using namespace esp;

namespace my_game
{
  struct ExampleVertex
  {
    glm::vec2 position;
    glm::vec3 color;
  };

  struct MVP
  {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  };

  static MVP get_new_mvp()
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    MVP mvp{};
    mvp.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    mvp.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mvp.proj = glm::perspective(glm::radians(45.0f), 1280.0f / ((float)720.0f), 0.1f, 10.0f);

    // mvp.view = glm::mat4(1.0f);
    // mvp.proj = glm::mat4(1.0f);

    return mvp;
  }

  static MVP get_new_mvp2()
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    MVP mvp{};
    mvp.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mvp.model = glm::translate(mvp.model, glm::vec3(1, 1, 0));
    //

    mvp.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mvp.proj = glm::perspective(glm::radians(45.0f), 1280.0f / ((float)720.0f), 0.1f, 10.0f);

    return mvp;
  }

  class ExampleLayer : public esp::Layer
  {
    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager_1;
    std::unique_ptr<EspUniformManager> m_uniform_manager_2;

    std::vector<ExampleVertex> m_square    = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                               { { -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                               { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                               { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } } };
    std::vector<uint32_t> m_square_indices = { 0, 1, 2, 2, 3, 0 };

    std::unique_ptr<EspVertexBuffers> m_vertex_buffers;
    std::unique_ptr<EspIndexBuffer> m_square_index_buffer;

   public:
    ExampleLayer()
    {
      auto pp_layout = EspUniformMetaData::create();
      pp_layout->establish_descriptor_set();
      pp_layout->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(MVP));

      auto builder = EspPipelineBuilder::create();
      builder->set_shaders("../shaders/Example/shader.vert.spv", "../shaders/Example/shader.frag.spv");
      builder->set_vertex_layouts({
          VTX_LAYOUT(
              sizeof(ExampleVertex),
              0,
              ESP_VERTEX_INPUT_RATE_VERTEX,
              ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(ExampleVertex, position)),
              ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(ExampleVertex, color))) /* VTX_LAYOUT*/
      }                                                                                            /* VTX_LAYOUTS */
      );
      builder->set_pipeline_layout(std::move(pp_layout));
      m_pipeline          = builder->build_pipeline();
      m_uniform_manager_1 = m_pipeline->create_uniform_manager();
      m_uniform_manager_1->build();
      m_uniform_manager_2 = m_pipeline->create_uniform_manager();
      m_uniform_manager_2->build();

      m_vertex_buffers = EspVertexBuffers::create();
      m_vertex_buffers->add(m_square.data(), sizeof(ExampleVertex), m_square.size());

      m_square_index_buffer = EspIndexBuffer::create(m_square_indices.data(), m_square_indices.size());
    }

   private:
    virtual ~ExampleLayer() override {}

    virtual void attach() override {}

    virtual void detach() override {}

    virtual void update() override
    {
      m_pipeline->attach();
      m_vertex_buffers->attach();

      auto mvp = get_new_mvp();
      m_uniform_manager_1->update_buffer_uniform(0, 0, 0, sizeof(MVP), &mvp);
      m_uniform_manager_1->attach();

      m_square_index_buffer->attach();
      EspCommandHandler::draw_indexed(m_square_indices.size());

      m_vertex_buffers->attach();

      mvp = get_new_mvp2();
      m_uniform_manager_2->update_buffer_uniform(0, 0, 0, sizeof(MVP), &mvp);
      m_uniform_manager_2->attach();

      m_square_index_buffer->attach();
      EspCommandHandler::draw_indexed(m_square_indices.size());
    }

    virtual void handle_event(esp::Event& event) override
    {
      esp::Event::try_hanlder<esp::KeyPressedEvent>(event,
                                                    ESP_BIND_EVENT_FOR_FUN(ExampleLayer::my_crazy_event_handler));
    }

    bool my_crazy_event_handler(esp::KeyPressedEvent& event)
    {
      // implementation ...
      ESP_INFO("You pressed the button {0}", event.get_code());

      /*
          true - propagate to lower layers
          false - this is the last layer of this event
      */
      return true;
    }
  };
} // namespace my_game

#endif // LAYERS_EXAMPLE_LAYER_H