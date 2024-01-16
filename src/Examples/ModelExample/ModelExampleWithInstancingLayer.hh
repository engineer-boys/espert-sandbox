#ifndef LAYERS_MODEL_EXAMPLE_WITH_INSTANCING_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_WITH_INSTANCING_LAYER_HH

#include "ModelExampleUtils.hh"

// libs
#include "Espert.hh"

using namespace esp;
using namespace esp::action;

namespace model_example_with_instancing
{
  struct CubeInstance
  {
    glm::mat4 model;

    CubeInstance(const glm::mat4& mat) : model{ mat } {}
  };

  class ModelExampleWithInstancingLayer : public Layer
  {
   private:
#define CUBES_X 3
#define CUBES_Z 9

    std::shared_ptr<EspShader> m_shader;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::shared_ptr<Mesh> m_cube_mesh;
    std::shared_ptr<Model> m_cube_model;
    std::shared_ptr<Node> m_main_cube_node;

    std::shared_ptr<Scene> m_scene;

   public:
    ModelExampleWithInstancingLayer(std::shared_ptr<Scene> scene) : m_scene{ std::move(scene) }
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(model_example::CameraPush));

      m_shader = ShaderSystem::acquire("Shaders/ModelExample/ModelExampleWithInstancing/shader");
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({
          Mesh::Vertex::get_vertex_layout(),
          VTX_LAYOUT(sizeof(CubeInstance),
                     1,
                     ESP_VERTEX_INPUT_RATE_INSTANCE,
                     ATTR(5, ESP_FORMAT_R32G32B32A32_SFLOAT, 0 * sizeof(glm::vec4)),
                     ATTR(6, ESP_FORMAT_R32G32B32A32_SFLOAT, 1 * sizeof(glm::vec4)),
                     ATTR(7, ESP_FORMAT_R32G32B32A32_SFLOAT, 2 * sizeof(glm::vec4)),
                     ATTR(8, ESP_FORMAT_R32G32B32A32_SFLOAT, 3 * sizeof(glm::vec4))),
      });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->build_worker();

      m_cube_mesh  = std::make_shared<Mesh>(model_example::create_cube_vertices());
      m_cube_model = std::make_shared<Model>(m_cube_mesh);

      std::array<std::shared_ptr<Entity>, CUBES_X * CUBES_Z> cubes{};
      for (auto& cube : cubes)
      {
        cube = m_scene->create_entity();
        cube->add_component<ModelComponent>(m_cube_model);
      }

      m_main_cube_node = Node::create();
      m_scene->get_root().add_child(m_main_cube_node);
      m_main_cube_node->attach_entity(cubes[0]);
      m_main_cube_node->translate({ 0.f, -.5f, 0.f });
      m_main_cube_node->scale(.5f);

      std::array<std::shared_ptr<Node>, CUBES_Z> temp_parents;
      for (int i = 0; i < CUBES_X; i++)
      {
        for (int j = 0; j < CUBES_Z; j++)
        {
          if (i == 0 && j == 0)
          {
            temp_parents[j] = m_main_cube_node;
            continue;
          }

          uint32_t parent_j;
          auto cube_node = Node::create();

          parent_j = std::max(j - 2, 0);

          temp_parents[parent_j]->add_child(cube_node);
          cube_node->attach_entity(cubes[i * CUBES_Z + j]);

          glm::vec3 translation = { 0.f, 0.f, 0.f };
          if (i != 0) { translation.x = (i % 2 == 0) ? -1.f : 1.f; }
          if (j != 0) { translation.z = (j % 2 == 0) ? -1.f : 1.f; }

          cube_node->translate(translation);
          cube_node->scale(.8f);

          if ((i != 0 && j > 2) || j != 0) { temp_parents[j] = cube_node; }
        }
      }

      std::vector<CubeInstance> instances{};
      m_main_cube_node->act([&instances](Node* node)
                            { instances.emplace_back(node->get_model_mat(ActionType::ABSOLUTE)); });
      m_cube_model->add_instance_buffer(instances);

      m_uniform_manager = m_shader->create_uniform_manager();
      m_uniform_manager->build();
    }

   private:
    virtual void update(float dt) override
    {
      m_shader->attach();

      m_main_cube_node->rotate(dt / 2, { 0, 1, 0 });

      std::vector<CubeInstance> instances{};
      m_main_cube_node->act([&instances](Node* node)
                            { instances.emplace_back(node->get_model_mat(ActionType::ABSOLUTE)); });
      m_cube_model->update_instance_buffer(instances, 0);

      auto camera = Scene::get_current_camera();

      model_example::CameraPush ubo{};
      ubo.view = camera->get_view();
      ubo.proj = camera->get_projection();

      m_uniform_manager->update_push_uniform(0, &ubo);
      m_uniform_manager->attach();

      m_cube_model->draw();
    }
  };
} // namespace model_example_with_instancing

#endif // LAYERS_MODEL_EXAMPLE_WITH_INSTANCING_LAYER_HH
