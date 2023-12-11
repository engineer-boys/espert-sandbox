#ifndef LAYERS_OBJ_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_OBJ_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace my_game
{
  struct VikingRoomUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  class ObjModelExampleLayer : public Layer
  {
   private:
    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::shared_ptr<Scene> m_scene;
    Camera m_camera{};

    std::shared_ptr<Node> m_viking_room_node;

   public:
    ObjModelExampleLayer()
    {
      m_scene            = Scene::create();
      m_viking_room_node = Node::create();
      m_scene->get_root().add_child(m_viking_room_node);

      auto viking_room = m_scene->create_entity("viking room");
      viking_room->add_component<TransformComponent>();
      viking_room->add_component<ModelComponent>(
          std::make_shared<Model>(Model::Builder{}.load_model("../resources/Models/viking_room.obj")));

      m_viking_room_node->attach_entity(viking_room);
      TransformAction::update_rotation(m_viking_room_node.get(), glm::radians(90.f), { 1.f, 0.f, 0.f }, ABSOLUTE);

      m_camera.set_position(glm::vec3(0.0f, -2.0f, 3.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(VikingRoomUniform));
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto builder = EspPipelineBuilder::create();

      builder->set_shaders("../resources/Shaders/ObjModelExample/shader.vert.spv",
                           "../resources/Shaders/ObjModelExample/shader.frag.spv");
      builder->set_vertex_layouts({ Model::Vertex::get_vertex_layout() });
      builder->set_pipeline_layout(std::move(uniform_meta_data));

      m_pipeline = builder->build_pipeline();

      m_uniform_manager = m_pipeline->create_uniform_manager();
      auto texture      = TextureSystem::acquire("Textures/viking_room.png");
      m_uniform_manager->load_texture(0, 1, texture);
      m_uniform_manager->build();
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);

      m_pipeline->attach();

      auto& model = m_viking_room_node->get_entity()->get_component<ModelComponent>();
      model.m_model_handle->attach();

      m_viking_room_node->act(
          [&dt](Node* node)
          {
            auto& transform = node->get_entity()->get_component<TransformComponent>();
            transform.reset();
            TransformAction::update_rotation(node, dt / 5, { 0.f, 0.f, 1.f }, ABSOLUTE);
          });

      m_camera.set_perspective(EspFrameManager::get_swap_chain_extent_aspect_ratio());

      auto& transform = m_viking_room_node->get_entity()->get_component<TransformComponent>();
      VikingRoomUniform ubo{};
      ubo.model = transform.get_model_mat();
      ubo.view  = m_camera.get_view();
      ubo.proj  = m_camera.get_projection();
      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(VikingRoomUniform), &ubo);
      m_uniform_manager->attach();

      EspCommandHandler::draw_indexed(model.m_model_handle->get_index_count());
    }
  };

} // namespace my_game

#endif // LAYERS_OBJ_MODEL_EXAMPLE_LAYER_HH