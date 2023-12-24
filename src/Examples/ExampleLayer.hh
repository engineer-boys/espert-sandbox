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

  struct ExamplePush
  {
    glm::vec2 m_pos;
    alignas(16) glm::vec3 m_color;
  };

  static MVP get_new_mvp()
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    MVP mvp{};
    mvp.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    mvp.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mvp.proj =
        glm::perspective(glm::radians(45.0f), EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

    return mvp;
  }

  static MVP get_new_mvp2()
  {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time       = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    MVP mvp{};
    mvp.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mvp.model = glm::translate(mvp.model, glm::vec3(1, 1, 0));
    //

    mvp.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    mvp.proj =
        glm::perspective(glm::radians(45.0f), EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

    return mvp;
  }

  class ExampleLayer : public esp::Layer
  {
    std::shared_ptr<EspShader> m_shader_1;
    std::shared_ptr<EspShader> m_shader_2;
    std::unique_ptr<EspUniformManager> m_uniform_manager_1;
    std::unique_ptr<EspUniformManager> m_uniform_manager_2;

    std::vector<ExampleVertex> m_square    = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                               { { -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                               { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                               { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } } };
    std::vector<uint32_t> m_square_indices = { 0, 2, 1, 2, 0, 3 };

    std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    std::unique_ptr<EspIndexBuffer> m_square_index_buffer;

    glm::vec2 m_push_pos{ 0.f, 0.f };
    glm::vec3 m_push_color{ 0.f, 0.f, 0.f };

    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

   public:
    ExampleLayer()
    {
      m_depth_block =
          EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

      m_final_product_plan = EspRenderPlan::build_final();
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });

      auto pp_layout = EspUniformMetaData::create();
      pp_layout->establish_descriptor_set();
      pp_layout->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(MVP));
      pp_layout->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE,
                                  offsetof(ExamplePush, m_pos),
                                  sizeof(glm::vec2));
      pp_layout->add_push_uniform(EspUniformShaderStage::ESP_FRAG_STAGE,
                                  offsetof(ExamplePush, m_color),
                                  sizeof(glm::vec3));

      m_shader_1 = ShaderSystem::acquire("Shaders/Example/shader");
      m_shader_1->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader_1->set_vertex_layouts({
          VTX_LAYOUT(
              sizeof(ExampleVertex),
              0,
              ESP_VERTEX_INPUT_RATE_VERTEX,
              ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(ExampleVertex, position)),
              ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(ExampleVertex, color))) /* VTX_LAYOUT*/
      }                                                                                            /* VTX_LAYOUTS */
      );
      m_shader_1->set_worker_layout(std::move(pp_layout));
      m_shader_1->build_worker();

      m_uniform_manager_1 = m_shader_1->create_uniform_manager();
      m_uniform_manager_1->build();

      auto pp_layout_2 = EspUniformMetaData::create();
      pp_layout_2->establish_descriptor_set();
      pp_layout_2->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(MVP));
      pp_layout_2->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE,
                                    offsetof(ExamplePush, m_pos),
                                    sizeof(glm::vec2));

      m_shader_2 = ShaderSystem::acquire(
          "Shaders/Example/shader",
          { { EspShaderStage::FRAGMENT, { { 0, true }, { 1, 0.5f }, { 2, 0.0f }, { 3, 1.0f } } } });
      m_shader_2->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader_2->set_vertex_layouts({
          VTX_LAYOUT(
              sizeof(ExampleVertex),
              0,
              ESP_VERTEX_INPUT_RATE_VERTEX,
              ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(ExampleVertex, position)),
              ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(ExampleVertex, color))) /* VTX_LAYOUT*/
      }                                                                                            /* VTX_LAYOUTS */
      );
      m_shader_2->set_worker_layout(std::move(pp_layout_2));
      m_shader_2->build_worker();

      m_uniform_manager_2 = m_shader_2->create_uniform_manager();
      m_uniform_manager_2->build();

      m_vertex_buffer = EspVertexBuffer::create(m_square.data(), sizeof(ExampleVertex), m_square.size());

      m_square_index_buffer = EspIndexBuffer::create(m_square_indices.data(), m_square_indices.size());
    }

   private:
    virtual ~ExampleLayer() override {}

    virtual void attach() override {}

    virtual void detach() override {}

    virtual void update(float dt) override
    {
      m_final_product_plan->begin_plan();
      {
        m_shader_1->attach();
        m_vertex_buffer->attach();

        m_push_pos.x += dt / 4;
        m_uniform_manager_1->update_push_uniform(0, &m_push_pos);
        m_push_color += dt / 8;
        m_uniform_manager_1->update_push_uniform(1, &m_push_color);

        auto mvp = get_new_mvp();
        m_uniform_manager_1->update_buffer_uniform(0, 0, 0, sizeof(MVP), &mvp);
        m_uniform_manager_1->attach();

        m_square_index_buffer->attach();
        EspJob::draw_indexed(m_square_indices.size());

        m_shader_2->attach();

        mvp = get_new_mvp2();
        m_uniform_manager_2->update_buffer_uniform(0, 0, 0, sizeof(MVP), &mvp);
        m_uniform_manager_2->attach();

        m_square_index_buffer->attach();
        EspJob::draw_indexed(m_square_indices.size());
      }
      m_final_product_plan->end_plan();
    }

    virtual void handle_event(esp::Event& event, float dt) override
    {
      esp::Event::try_handler<esp::KeyPressedEvent>(event,
                                                    ESP_BIND_EVENT_FOR_FUN(ExampleLayer::my_crazy_event_handler));
    }

    bool my_crazy_event_handler(esp::KeyPressedEvent& event)
    {
      // implementation ...
      ESP_INFO("You pressed the button {0}", event.get_code());

      /*
          false - propagate to lower layers
          true - this is the last layer of this event
      */
      return true;
    }
  };
} // namespace my_game

#endif // LAYERS_EXAMPLE_LAYER_H