#ifndef LAYERS_CUBE_EXAMPLE_LAYER_H
#define LAYERS_CUBE_EXAMPLE_LAYER_H

// libs
#include "Espert.hh"

using namespace esp;

namespace my_game
{
  class ExampleLayer : public esp::Layer
  {
    std::unique_ptr<ExamplePipelineLayout> m_pipeline_layout;
    std::unique_ptr<ExamplePipeline> m_pipeline;

    std::vector<ExampleVertex> m_left_triangle = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                                   { { -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                                   { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } } };
    std::unique_ptr<EspVertexBuffer> m_left_triangle_vertex_buffer;

    std::vector<ExampleVertex> m_right_triangle = { { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                                    { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
                                                    { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } } };
    std::unique_ptr<EspVertexBuffer> m_right_triangle_vertex_buffer;

    std::vector<ExampleVertex> m_square = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                            { { -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                            { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                            { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } } };
    std::unique_ptr<EspVertexBuffer> m_square_buffer;
    std::vector<uint32_t> m_square_indices = { 0, 1, 2, 2, 3, 0 };
    std::unique_ptr<EspIndexBuffer> m_square_index_buffer;

    // std::unique_ptr<EspVertexBuffers> m_vertex_buffers;

   public:
    ExampleLayer()
    {
      ExamplePipelineConfigInfo pipeline_config{};
      ExamplePipeline::default_pipeline_config_info(pipeline_config);

      ExamplePipeline::Builder builder;
      builder.set_vert_shader_path("../shaders/shader.vert.spv").set_frag_shader_path("../shaders/shader.frag.spv");

      m_pipeline_layout = builder.build_pipeline_layout(pipeline_config);
      m_pipeline        = builder.build_pipeline(pipeline_config);

      //      m_vertex_buffers = EspVertexBuffers::create();
      //      m_vertex_buffers->add(m_vertices.data(), sizeof(ExampleVertex), 3);
      //      m_vertex_buffers->add(m_vertices1.data(), sizeof(ExampleVertex), 3);

      m_left_triangle_vertex_buffer  = EspVertexBuffer::create(m_left_triangle.data(), sizeof(ExampleVertex), 3);
      m_right_triangle_vertex_buffer = EspVertexBuffer::create(m_right_triangle.data(), sizeof(ExampleVertex), 3);

      m_square_buffer       = EspVertexBuffer::create(m_square.data(), sizeof(ExampleVertex), 4);
      m_square_index_buffer = EspIndexBuffer::create(m_square_indices.data(), 6);
    }

   private:
    virtual ~ExampleLayer() override {}

    virtual void attach() override {}

    virtual void detach() override {}

    virtual void update() override
    {
      m_pipeline->bind();

      //      m_left_triangle_vertex_buffer->attach();
      //      EspCommandHandler::draw(3);

      //      m_right_triangle_vertex_buffer->attach();
      //      EspCommandHandler::draw(3);

      m_square_buffer->attach();
      m_square_index_buffer->attach();
      EspCommandHandler::draw_indexed(6);
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

#endif // LAYERS_CUBE_EXAMPLE_LAYER_H
