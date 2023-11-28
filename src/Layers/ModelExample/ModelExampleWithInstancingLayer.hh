#ifndef LAYERS_MODEL_EXAMPLE_WITH_INSTANCING_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_WITH_INSTANCING_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;
using namespace esp::action;

namespace model_example_with_instancing
{
  static std::vector<Model::Vertex> create_cube_vertices();

  struct CubeInstance
  {
    glm::mat4 model;

    CubeInstance(const glm::mat4& mat) : model{ mat } {}
  };

  struct CubeUniform
  {
    glm::mat4 view;
    glm::mat4 proj;
  };

  class ModelExampleWithInstancingLayer : public Layer
  {
   private:
#define CUBES 3

    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::shared_ptr<Scene> m_scene;
    Camera m_camera{};

    std::shared_ptr<Model> m_cube_model;
    std::shared_ptr<SceneNode> m_main_cube_node;

   public:
    ModelExampleWithInstancingLayer()
    {
      m_scene = Scene::create();

      m_camera.set_position(glm::vec3{ 0.f, -1.f, -5.f });
      m_camera.look_at(glm::vec3{ 0.f, 0.f, 0.f });
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      m_cube_model = std::make_unique<Model>(create_cube_vertices());

      std::array<std::shared_ptr<Entity>, CUBES> cubes{};
      for (auto& cube : cubes)
      {
        cube = m_scene->create_entity();
        cube->add_component<TransformComponent>();
        cube->add_component<ModelComponent>(m_cube_model);
      }

      m_main_cube_node = SceneNode::create();
      m_scene->get_root().add_child(m_main_cube_node);
      m_main_cube_node->attach_entity(cubes[0]);
      TransformAction::set_translation(m_main_cube_node.get(), glm::vec3{ 0.f, 0.f, 2.f }, RELATIVE);
      TransformAction::set_scale(m_main_cube_node.get(), .5f, RELATIVE);

      for (int i = 1; i < cubes.size(); i++)
      {
        auto small_cube_node = SceneNode::create();
        m_main_cube_node->add_child(small_cube_node);
        small_cube_node->set_parent(m_main_cube_node);
        small_cube_node->attach_entity(cubes[i]);
        TransformAction::set_translation(small_cube_node.get(),
                                         glm::vec3{ i % 2 == 0 ? -1.f : 1.f, 0.f, 0.f },
                                         RELATIVE);
        TransformAction::set_scale(small_cube_node.get(), .5f, RELATIVE);
      }

      std::vector<CubeInstance> instances{};
      m_main_cube_node->act(
          [&instances](SceneNode* node)
          {
            auto& transform = node->get_entity()->get_component<TransformComponent>();
            instances.emplace_back(transform.get_model_mat());
          });
      m_cube_model->add_instance_buffer(instances);

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(CubeUniform));

      auto builder = EspPipelineBuilder::create();

      builder->set_shaders("../resources/Shaders/ModelExample/ModelExampleWithInstancing/shader.vert.spv",
                           "../resources/Shaders/ModelExample/ModelExampleWithInstancing/shader.frag.spv");
      builder->set_vertex_layouts({
          Model::Vertex::get_vertex_layout(),
          VTX_LAYOUT(sizeof(CubeInstance),
                     1,
                     ESP_VERTEX_INPUT_RATE_INSTANCE,
                     ATTR(4, ESP_FORMAT_R32G32B32A32_SFLOAT, 0 * sizeof(glm::vec4)),
                     ATTR(5, ESP_FORMAT_R32G32B32A32_SFLOAT, 1 * sizeof(glm::vec4)),
                     ATTR(6, ESP_FORMAT_R32G32B32A32_SFLOAT, 2 * sizeof(glm::vec4)),
                     ATTR(7, ESP_FORMAT_R32G32B32A32_SFLOAT, 3 * sizeof(glm::vec4))),
      });
      builder->set_pipeline_layout(std::move(uniform_meta_data));

      m_pipeline = builder->build_pipeline();

      m_uniform_manager = m_pipeline->create_uniform_manager();
      m_uniform_manager->build();
    }

   private:
    virtual void update(float dt) override
    {
      Camera::set_current_camera(&m_camera);

      m_pipeline->attach();

      std::vector<CubeInstance> instances{};
      m_main_cube_node->act(
          [&instances, dt](SceneNode* node)
          {
            auto& transform = node->get_entity()->get_component<TransformComponent>();
            transform.reset_model_mat();

            TransformAction::update_rotation(node, dt / 2, glm::vec3{ 0.f, 1.f, 0.f }, ABSOLUTE);
            TransformAction::translate(node, ABSOLUTE);
            TransformAction::scale(node, ABSOLUTE);

            instances.emplace_back(transform.get_model_mat());
          });

      m_cube_model->update_instance_buffer(instances, 0);

      m_camera.set_perspective(EspFrameManager::get_swap_chain_extent_aspect_ratio());

      CubeUniform ubo{};
      ubo.view = m_camera.get_view();
      ubo.proj = m_camera.get_projection();

      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(CubeUniform), &ubo);
      m_uniform_manager->attach();

      m_cube_model->attach();
      EspCommandHandler::draw(m_cube_model->get_vertex_count(), CUBES);
    }
  };

  static std::vector<Model::Vertex> create_cube_vertices()
  {
    std::vector<Model::Vertex> vertices{

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
      { { -.5f, .5f, -0.5f }, { .1f, .8f, .1f } },
      { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },
      { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
      { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },
      { { .5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
    };

    return vertices;
  }

} // namespace model_example_with_instancing

#endif // LAYERS_MODEL_EXAMPLE_WITH_INSTANCING_LAYER_HH
