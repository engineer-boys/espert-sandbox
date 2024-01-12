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
    static auto start_time = std::chrono::high_resolution_clock::now();

    auto current_time = std::chrono::high_resolution_clock::now();
    float time        = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    InstancingExampleUniform mvp{};
    mvp.model = glm::scale(glm::mat4(1.0f), { 0.05f, 0.05f, 0.05f });
    mvp.model = glm::rotate(mvp.model, time * glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    mvp.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.25f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    mvp.proj =
        glm::perspective(glm::radians(45.0f), EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

    return mvp;
  }

  class InstancingExampleLayer : public esp::Layer
  {
    std::shared_ptr<EspShader> m_shader;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::vector<InstancingExampleVertex> m_square = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                                      { { -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                                      { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                                      { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } } };
    std::vector<uint32_t> m_square_indices        = { 0, 2, 1, 2, 0, 3 };
    std::vector<glm::vec2> m_instance_pos{};

    std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    std::unique_ptr<EspVertexBuffer> m_instance_buffer;
    std::unique_ptr<EspIndexBuffer> m_square_index_buffer;

    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

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
      m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                           EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                           EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

      m_final_product_plan = EspRenderPlan::create_final();
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });
      m_final_product_plan->build();

      auto pp_layout = EspUniformMetaData::create();
      pp_layout->establish_descriptor_set();
      pp_layout->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(InstancingExampleUniform));

      m_shader = ShaderSystem::acquire("Shaders/InstancingExample/shader");
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({
          VTX_LAYOUT(sizeof(ExampleVertex),
                     0,
                     ESP_VERTEX_INPUT_RATE_VERTEX,
                     ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(InstancingExampleVertex, position)),
                     ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(InstancingExampleVertex, color))),
          VTX_LAYOUT(sizeof(glm::vec2),
                     1,
                     ESP_VERTEX_INPUT_RATE_INSTANCE,
                     ATTR(2, ESP_FORMAT_R32G32_SFLOAT, 0)) /* VTX_LAYOUT*/
      }                                                    /* VTX_LAYOUTS */
      );
      m_shader->set_worker_layout(std::move(pp_layout));
      m_shader->build_worker();

      m_uniform_manager = m_shader->create_uniform_manager();
      m_uniform_manager->build();

      m_vertex_buffer = EspVertexBuffer::create(m_square.data(), sizeof(ExampleVertex), m_square.size());
      m_instance_buffer =
          EspVertexBuffer::create(m_instance_pos.data(), sizeof(m_instance_pos[0]), m_instance_pos.size());

      m_square_index_buffer = EspIndexBuffer::create(m_square_indices.data(), m_square_indices.size());
    }

   private:
    virtual ~InstancingExampleLayer() override {}

    virtual void attach() override {}

    virtual void detach() override {}

    virtual void update(float dt) override
    {
      m_final_product_plan->begin_plan();
      {
        m_shader->attach();
        m_vertex_buffer->attach_instanced(*m_instance_buffer);
        m_square_index_buffer->attach();

        auto mvp = get_new_instancing_example_uniform();
        m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(InstancingExampleUniform), &mvp);
        m_uniform_manager->attach();

        EspJob::draw_indexed(m_square_indices.size(), m_instance_pos.size());
      }
      m_final_product_plan->end_plan();
    }
  };
} // namespace my_game

#endif // LAYERS_EXAMPLE_INSTANCING_LAYER_H
