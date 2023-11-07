#ifndef LAYERS_OBJ_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_OBJ_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

static std::unique_ptr<ModelComponent> create_cube_model();

namespace my_game
{
  struct VikingRoomUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  class VikingRoom
  {
   public:
    TransformComponent m_transform;
    std::unique_ptr<ModelComponent> m_model;

    VikingRoom()
    {
      ModelComponent::Builder builder{};
      builder.load_model("../resources/Models/viking_room.obj");
      m_model = std::make_unique<ModelComponent>(builder);
    }

    VikingRoomUniform update()
    {
      float dt = .005f;

      auto& rot_y = m_transform.m_rotation.y;
      rot_y       = glm::mod(rot_y + dt, glm::two_pi<float>());
      VikingRoomUniform ubo{};
      ubo.model = m_transform.get_model_mat();
      ubo.view  = glm::lookAt(glm::vec3(0.0f, -2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
      ubo.proj =
          glm::perspective(glm::radians(50.0f), EspFrameManager::get_swap_chain_extent_aspect_ratio(), 0.1f, 10.0f);

      return ubo;
    }
  };

  class ObjModelExampleLayer : public Layer
  {
   private:
    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    VikingRoom m_viking_room;

   public:
    ObjModelExampleLayer()
    {
      m_viking_room                        = VikingRoom();
      m_viking_room.m_transform.m_rotation = { glm::radians(90.f), glm::radians(-45.f), 0.0f };

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(VikingRoomUniform));
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto builder = EspPipelineBuilder::create();

      builder->set_shaders("../resources/Shaders/ObjModelExample/shader.vert.spv",
                           "../resources/Shaders/ObjModelExample/shader.frag.spv");
      builder->set_vertex_layouts({ ModelComponent::Vertex::get_vertex_layout() });
      builder->set_pipeline_layout(std::move(uniform_meta_data));

      m_pipeline = builder->build_pipeline();

      m_uniform_manager = m_pipeline->create_uniform_manager();
      m_uniform_manager->load_texture(0, 1, "../resources/Textures/viking_room.png");
      m_uniform_manager->build();
    }

   private:
    virtual void update() override
    {
      m_pipeline->attach();
      m_viking_room.m_model->attach();

      auto ubo = m_viking_room.update();
      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(VikingRoomUniform), &ubo);
      m_uniform_manager->attach();

      EspCommandHandler::draw_indexed(m_viking_room.m_model->get_index_count());
    }
  };

} // namespace my_game

#endif // LAYERS_OBJ_MODEL_EXAMPLE_LAYER_HH
