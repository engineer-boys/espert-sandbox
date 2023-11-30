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

  struct CubeUniform
  {
    glm::mat4 view;
    glm::mat4 proj;
  };

  class ModelExampleWithInstancingLayer : public Layer
  {
   private:
#define CUBES_X 3
#define CUBES_Z 9

    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::shared_ptr<Model> m_cube_model;
    std::shared_ptr<Node> m_main_cube_node;

    std::shared_ptr<Scene> m_scene;

   public:
    ModelExampleWithInstancingLayer(std::shared_ptr<Scene> scene) : m_scene{ std::move(scene) }
    {
      m_cube_model = std::make_unique<Model>(model_example::create_cube_vertices());

      std::array<std::shared_ptr<Entity>, CUBES_X * CUBES_Z> cubes{};
      for (auto& cube : cubes)
      {
        cube = m_scene->create_entity();
        cube->add_component<TransformComponent>();
        cube->add_component<ModelComponent>(m_cube_model);
      }

      m_main_cube_node = Node::create();
      m_scene->get_root().add_child(m_main_cube_node);
      m_main_cube_node->attach_entity(cubes[0]);
      TransformAction::set_translation(m_main_cube_node.get(), glm::vec3{ 0.f, .5f, 2.f }, RELATIVE);
      TransformAction::set_scale(m_main_cube_node.get(), .5f, RELATIVE);

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
          cube_node->set_parent(temp_parents[parent_j]);
          cube_node->attach_entity(cubes[i * CUBES_Z + j]);

          glm::vec3 translation = { 0.f, 0.f, 0.f };
          if (i != 0) { translation.x = (i % 2 == 0) ? -1.f : 1.f; }
          if (j != 0) { translation.z = (j % 2 == 0) ? -1.f : 1.f; }

          TransformAction::set_translation(cube_node.get(), translation, RELATIVE);
          TransformAction::set_scale(cube_node.get(), .8f, RELATIVE);

          if ((i != 0 && j > 2) || j != 0) { temp_parents[j] = cube_node; }
        }
      }

      std::vector<CubeInstance> instances{};
      m_main_cube_node->act(
          [&instances](Node* node)
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
      m_pipeline->attach();

      m_main_cube_node->act(TransformAction::reset);
      m_main_cube_node->act(TransformAction::translate, ABSOLUTE);
      m_main_cube_node->act(TransformAction::update_rotation, dt / 2, glm::vec3{ 0.f, 1.f, 0.f }, ABSOLUTE);
      m_main_cube_node->act(TransformAction::scale, ABSOLUTE);

      std::vector<CubeInstance> instances{};                                           //
      m_main_cube_node->act(                                                           //
          [&instances](Node* node)                                                     //
          {                                                                            // TODO: This part is likely
            auto& transform = node->get_entity()->get_component<TransformComponent>(); // going to end up in renderer
            instances.emplace_back(transform.get_model_mat());                         //
          });                                                                          //
      m_cube_model->update_instance_buffer(instances, 0);                              //

      auto camera = Scene::get_current_camera();

      CubeUniform ubo{};
      ubo.view = camera->get_view();
      ubo.proj = camera->get_projection();

      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(CubeUniform), &ubo);
      m_uniform_manager->attach();

      m_cube_model->attach();
      EspCommandHandler::draw(m_cube_model->get_vertex_count(), CUBES_X * CUBES_Z);
    }
  };
} // namespace model_example_with_instancing

#endif // LAYERS_MODEL_EXAMPLE_WITH_INSTANCING_LAYER_HH
