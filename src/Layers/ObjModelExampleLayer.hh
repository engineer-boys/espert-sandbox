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

    VikingRoomUniform update(Camera& camera, float dt)
    {
      auto& rot_y = m_transform.m_rotation.y;
      rot_y       = glm::mod(rot_y + dt / 5, glm::two_pi<float>());

      camera.set_perspective(EspFrameManager::get_swap_chain_extent_aspect_ratio());

      VikingRoomUniform ubo{};
      ubo.model = m_transform.get_model_mat();
      ubo.view  = camera.get_view();
      ubo.proj  = camera.get_projection();

      return ubo;
    }
  };

  class ObjModelExampleLayer : public Layer
  {
   private:
    std::unique_ptr<EspPipeline> m_pipeline;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    Camera m_camera{};

    VikingRoom m_viking_room;

   public:
    ObjModelExampleLayer()
    {
      m_camera.set_position(glm::vec3(0.0f, -2.0f, 3.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

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
    virtual void update(float dt) override
    {
      Camera::set_current_camera(&m_camera);

      m_pipeline->attach();
      m_viking_room.m_model->attach();

      auto ubo = m_viking_room.update(m_camera, dt);
      m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(VikingRoomUniform), &ubo);
      m_uniform_manager->attach();

      EspCommandHandler::draw_indexed(m_viking_room.m_model->get_index_count());
    }
  };

} // namespace my_game

#endif // LAYERS_OBJ_MODEL_EXAMPLE_LAYER_HH
