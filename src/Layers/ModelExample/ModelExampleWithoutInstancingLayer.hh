#ifndef LAYERS_MODEL_EXAMPLE_WITHOUT_INSTANCING_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_WITHOUT_INSTANCING_LAYER_HH

#include "ModelExampleUtils.hh"

// libs
#include "Espert.hh"

using namespace esp;
using namespace esp::action;

namespace model_example_without_instancing
{
  struct CubeUniform
  {
    glm::mat4 model;
  };

  class ModelExampleWithoutInstancingLayer : public Layer
  {
   private:
    std::unique_ptr<EspPipeline> m_pipeline;
    std::array<std::unique_ptr<EspUniformManager>, 3> m_uniform_managers{};

    std::shared_ptr<Model> m_cube_model;
    std::shared_ptr<Node> m_main_cube_node;

    std::shared_ptr<Scene> m_scene;

   public:
    ModelExampleWithoutInstancingLayer(std::shared_ptr<Scene> scene) : m_scene{ std::move(scene) }
    {
      m_cube_model = std::make_unique<Model>(model_example::create_cube_vertices());

      std::array<std::shared_ptr<Entity>, 3> cubes{};
      for (auto& cube : cubes)
      {
        cube = m_scene->create_entity();
        cube->add_component<TransformComponent>();
        cube->add_component<ModelComponent>(m_cube_model);
      }

      m_main_cube_node = Node::create();
      m_scene->get_root().add_child(m_main_cube_node);
      m_main_cube_node->attach_entity(cubes[0]);
      TransformAction::set_translation(m_main_cube_node.get(), glm::vec3{ 0.f, -.5f, 2.f }, RELATIVE);
      TransformAction::set_scale(m_main_cube_node.get(), .5f, RELATIVE);

      for (int i = 1; i < cubes.size(); i++)
      {
        auto small_cube_node = Node::create();
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
      uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(model_example::CameraPush));
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(CubeUniform));

      auto builder = EspPipelineBuilder::create();

      builder->set_shaders("../resources/Shaders/ModelExample/ModelExampleWithoutInstancing/shader.vert.spv",
                           "../resources/Shaders/ModelExample/ModelExampleWithoutInstancing/shader.frag.spv");
      builder->set_vertex_layouts({ Model::Vertex::get_vertex_layout() });
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
      m_pipeline->attach();

      m_cube_model->attach();

      m_main_cube_node->act(
          [dt](Node* node)
          {
            auto& transform = node->get_entity()->get_component<TransformComponent>();
            transform.reset();

            TransformAction::update_rotation(node, -dt / 2, glm::vec3{ 0.f, 1.f, 0.f }, ABSOLUTE);
            TransformAction::translate(node, ABSOLUTE);
            TransformAction::scale(node, ABSOLUTE);
          });

      int i = 0;
      m_main_cube_node->act(
          [this, &i](Node* node)
          {
            auto& transform = node->get_entity()->get_component<TransformComponent>();

            CubeUniform ubo{};
            ubo.model = transform.get_model_mat();

            m_uniform_managers[i]->update_buffer_uniform(0, 0, 0, sizeof(CubeUniform), &ubo);
            m_uniform_managers[i]->attach();

            auto& model = node->get_entity()->get_component<ModelComponent>();
            EspCommandHandler::draw(model.m_model_handle->get_vertex_count());

            i++;
          });
    }
  };
} // namespace model_example_without_instancing

#endif // LAYERS_MODEL_EXAMPLE_WITHOUT_INSTANCING_LAYER_HH
