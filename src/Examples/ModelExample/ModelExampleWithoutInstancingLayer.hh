#ifndef LAYERS_MODEL_EXAMPLE_WITHOUT_INSTANCING_LAYER_HH
#define LAYERS_MODEL_EXAMPLE_WITHOUT_INSTANCING_LAYER_HH

#include "ModelExampleUtils.hh"

// libs
#include "Espert.hh"

using namespace esp;

namespace model_example_without_instancing
{
  struct CubeUniform
  {
    glm::mat4 model;
  };

  class ModelExampleWithoutInstancingLayer : public Layer
  {
#define CUBES 5
   private:
    std::shared_ptr<EspShader> m_shader;

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Node> m_main_cube_node;

   public:
    ModelExampleWithoutInstancingLayer(std::shared_ptr<Scene> scene) : m_scene{ std::move(scene) }
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(model_example::CameraPush));
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(CubeUniform));

      m_shader = ShaderSystem::acquire("Shaders/ModelExample/ModelExampleWithoutInstancing/shader");
      m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->build_worker();

      auto cube_mesh  = std::make_shared<Mesh>(model_example::create_cube_vertices());
      auto cube_model = std::make_shared<Model>(cube_mesh);

      std::array<std::shared_ptr<Entity>, CUBES> cubes{};
      for (auto& cube : cubes)
      {
        cube = m_scene->create_entity();
        cube->add_component<ModelComponent>(cube_model, m_shader);
      }

      m_main_cube_node = Node::create();
      m_scene->get_root().add_child(m_main_cube_node);
      m_main_cube_node->attach_entity(cubes[0]);
      m_main_cube_node->translate({ 0.f, .5f, -4.f });

      std::array<std::shared_ptr<Node>, CUBES> nodes{};
      nodes[0] = m_main_cube_node;
      for (int i = 1; i < CUBES; i++)
      {
        nodes[i] = Node::create();
        nodes[i - 1]->add_child(nodes[i]);
        nodes[i]->attach_entity(cubes[i]);
        nodes[i]->translate({ 1.5f / i, 0.f, 0.f });
        nodes[i]->scale(.5f);
      }
    }

   private:
    virtual void update(float dt) override
    {
      m_shader->attach();

      m_main_cube_node->rotate(-dt, { 0, 1, 0 });
      m_main_cube_node->set_translation(
          model_example::move_in_circle(m_main_cube_node->get_translation(), { 0, 1, 0 }, glm::radians(dt * 2)));

      m_main_cube_node->act(
          [](Node* node)
          {
            CubeUniform ubo{};
            ubo.model = node->get_model_mat(ActionType::ESP_ABSOLUTE);

            auto& uniform_manager = node->get_entity()->get_component<ModelComponent>().get_uniform_manager();
            uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(CubeUniform), &ubo);
          });

      m_scene->render();
    }
  };
} // namespace model_example_without_instancing

#endif // LAYERS_MODEL_EXAMPLE_WITHOUT_INSTANCING_LAYER_HH
