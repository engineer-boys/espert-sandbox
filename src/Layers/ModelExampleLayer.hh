#ifndef LAYERS_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;
using namespace esp::action;

static std::vector<ModelComponent::Vertex> create_cube_vertices();

namespace my_game
{
  struct CubeUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  class ModelExampleLayer : public Layer
  {
   private:
    std::unique_ptr<EspPipeline> m_pipeline;
    std::array<std::unique_ptr<EspUniformManager>, 3> m_uniform_managers{};

    std::shared_ptr<Scene> m_scene;
    Camera m_camera{};

    std::shared_ptr<SceneNode> m_main_cube_node;

   public:
    ModelExampleLayer()
    {
      m_scene = Scene::create();

      m_camera.set_position(glm::vec3{ 0.f, -1.f, -5.f });
      m_camera.look_at(glm::vec3{ 0.f, 0.f, 0.f });
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      std::vector<ModelComponent::Vertex> cube_vertices = create_cube_vertices();
      std::array<std::shared_ptr<Entity>, 3> cubes{};
      for (auto& cube : cubes)
      {
        cube = m_scene->create_entity();
        cube->add_component<TransformComponent>();
        cube->add_component<ModelComponent>(create_cube_vertices());
      }

      m_main_cube_node = SceneNode::create();
      m_scene->get_root().add_child(m_main_cube_node);
      m_main_cube_node->attach_entity(cubes[0]);

      m_main_cube_node->get_entity()->get_component<TransformComponent>();
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

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(CubeUniform));

      auto builder = EspPipelineBuilder::create();

      builder->set_shaders("../resources/Shaders/ModelExample/shader.vert.spv",
                           "../resources/Shaders/ModelExample/shader.frag.spv");
      builder->set_vertex_layouts({ ModelComponent::Vertex::get_vertex_layout() });
      builder->set_pipeline_layout(std::move(uniform_meta_data));

      m_pipeline = builder->build_pipeline();

      for (auto& manager : m_uniform_managers)
      {
        manager = m_pipeline->create_uniform_manager();
        manager->build();
      }
    }

   private:
    virtual void update(float dt) override
    {
      Camera::set_current_camera(&m_camera);

      m_pipeline->attach();

      auto& model = m_main_cube_node->get_entity()->get_component<ModelComponent>();
      model.attach();

      m_main_cube_node->act(
          [dt](SceneNode* node)
          {
            auto& transform = node->get_entity()->get_component<TransformComponent>();

            transform.reset_model_mat();

            TransformAction::update_rotation(node, dt / 2, glm::vec3{ 0.f, 1.f, 0.f }, ABSOLUTE);
            TransformAction::translate(node, ABSOLUTE);
            TransformAction::scale(node, ABSOLUTE);
          });

      m_camera.set_perspective(EspFrameManager::get_swap_chain_extent_aspect_ratio());

      int i = 0;
      m_main_cube_node->act(
          [this, &i](SceneNode* node)
          {
            auto& transform = node->get_entity()->get_component<TransformComponent>();

            CubeUniform ubo{};
            ubo.model = transform.get_model_mat();
            ubo.view  = m_camera.get_view();
            ubo.proj  = m_camera.get_projection();

            m_uniform_managers[i]->update_buffer_uniform(0, 0, 0, sizeof(CubeUniform), &ubo);
            m_uniform_managers[i]->attach();

            auto& model = node->get_entity()->get_component<ModelComponent>();
            EspCommandHandler::draw(model.get_vertex_count());

            i++;
          });
    }
  };

} // namespace my_game

static std::vector<ModelComponent::Vertex> create_cube_vertices()
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
    { { -.5f, .5f, -0.5f }, { .1f, .8f, .1f } },
    { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },
    { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
    { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },
    { { .5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
  };

  return vertices;
}

#endif // LAYERS_MODEL_EXAMPLE_LAYER_HH
