#ifndef LAYERS_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

static std::unique_ptr<ModelComponent> create_cube_model();

namespace my_game
{
  struct CubeUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  class Cube
  {
   public:
    TransformComponent m_transform;
    std::unique_ptr<ModelComponent> m_model;

    Cube() { m_model = create_cube_model(); }

    CubeUniform update()
    {
      float dt = .01f;

      auto& rot_y = m_transform.m_rotation.y;
      auto& rot_x = m_transform.m_rotation.x;
      rot_y       = glm::mod(rot_x + dt, glm::two_pi<float>());
      rot_x       = glm::mod(rot_y + dt / 2, glm::two_pi<float>());
      CubeUniform ubo{};
      ubo.model = m_transform.get_model_mat();
      ubo.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
      ubo.proj =
          glm::perspective(glm::radians(50.0f), EspFrameManager::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

      return ubo;
    }
  };

  class ModelExampleLayer : public Layer
  {
   private:
    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    Cube m_cube;

   public:
    ModelExampleLayer()
    {
      m_cube                           = Cube();
      m_cube.m_transform.m_translation = { 0.0f, 0.0f, 0.5f };
      m_cube.m_transform.m_scale       = { 0.5f, 0.5f, 0.5f };

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(CubeUniform));

      auto builder = EspPipelineBuilder::create();

      builder->set_shaders("../resources/Shaders/ModelExample/shader.vert.spv",
                           "../resources/Shaders/ModelExample/shader.frag.spv");
      builder->set_vertex_layouts({ ModelComponent::Vertex::get_vertex_layout() });
      builder->set_pipeline_layout(std::move(uniform_meta_data));

      m_pipeline        = builder->build_pipeline();
      m_uniform_manager = m_pipeline->create_uniform_manager();
      m_uniform_manager->build();
    }

   private:
    virtual void update(float dt) override
    {
      m_pipeline->attach();
      m_cube.m_model->attach();

      auto ubo = m_cube.update();
      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(CubeUniform), &ubo);
      m_uniform_manager->attach();

      EspCommandHandler::draw(m_cube.m_model->get_vertex_count());
    }
  };

} // namespace my_game

static std::unique_ptr<ModelComponent> create_cube_model()
{
  std::vector<ModelComponent::Vertex> vertices{

    // left face (white)
    { { -.5f, -.5f, -.5f }, { .9f, .9f, .9f } },
    { { -.5f, -.5f, .5f }, { .9f, .9f, .9f } },
    { { -.5f, .5f, .5f }, { .9f, .9f, .9f } },
    { { -.5f, -.5f, -.5f }, { .9f, .9f, .9f } },
    { { -.5f, .5f, .5f }, { .9f, .9f, .9f } },
    { { -.5f, .5f, -.5f }, { .9f, .9f, .9f } },

    // right face (yellow)
    { { .5f, -.5f, -.5f }, { .8f, .8f, .1f } },
    { { .5f, .5f, .5f }, { .8f, .8f, .1f } },
    { { .5f, -.5f, .5f }, { .8f, .8f, .1f } },
    { { .5f, -.5f, -.5f }, { .8f, .8f, .1f } },
    { { .5f, .5f, -.5f }, { .8f, .8f, .1f } },
    { { .5f, .5f, .5f }, { .8f, .8f, .1f } },

    // top face (orange, remember y axis points down)
    { { -.5f, -.5f, -.5f }, { .9f, .6f, .1f } },
    { { .5f, -.5f, .5f }, { .9f, .6f, .1f } },
    { { -.5f, -.5f, .5f }, { .9f, .6f, .1f } },
    { { -.5f, -.5f, -.5f }, { .9f, .6f, .1f } },
    { { .5f, -.5f, -.5f }, { .9f, .6f, .1f } },
    { { .5f, -.5f, .5f }, { .9f, .6f, .1f } },

    // bottom face (red)
    { { -.5f, .5f, -.5f }, { .8f, .1f, .1f } },
    { { -.5f, .5f, .5f }, { .8f, .1f, .1f } },
    { { .5f, .5f, .5f }, { .8f, .1f, .1f } },
    { { -.5f, .5f, -.5f }, { .8f, .1f, .1f } },
    { { .5f, .5f, .5f }, { .8f, .1f, .1f } },
    { { .5f, .5f, -.5f }, { .8f, .1f, .1f } },

    // nose face (blue)
    { { -.5f, -.5f, 0.5f }, { .1f, .1f, .8f } },
    { { .5f, .5f, 0.5f }, { .1f, .1f, .8f } },
    { { -.5f, .5f, 0.5f }, { .1f, .1f, .8f } },
    { { -.5f, -.5f, 0.5f }, { .1f, .1f, .8f } },
    { { .5f, -.5f, 0.5f }, { .1f, .1f, .8f } },
    { { .5f, .5f, 0.5f }, { .1f, .1f, .8f } },

    // tail face (green)
    { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
    { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },
    { { -.5f, .5f, -0.5f }, { .1f, .8f, .1f } },
    { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
    { { .5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
    { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },

  };

  return std::make_unique<ModelComponent>(vertices);
}

#endif // LAYERS_MODEL_EXAMPLE_LAYER_HH
