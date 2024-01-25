#ifndef LAYERS_VIKING_ROOM_OBJ_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_VIKING_ROOM_OBJ_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace obj_example
{
  struct VikingRoomUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  class VikingRoomObjModelExampleLayer : public Layer
  {
   private:
    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_product_plan;

    ModelParams m_model_params = { .m_position                = true,
                                    .m_normal                  = true,
                                    .m_tex_coord               = true,
                                    .m_material_texture_layout = { { 1, 0, EspTextureType::ALBEDO } },
                                    .m_load_process_flags      = EspProcessFlipUVs };

    std::shared_ptr<Scene> m_scene;
    Camera m_camera{};
    std::shared_ptr<Node> m_viking_room_node;

   public:
    VikingRoomObjModelExampleLayer()
    {
      // create render plan
      m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                           EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT,
                                           EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

      m_final_product_plan =
          EspRenderPlan::create_final({ 0.1f, 0.1f, 0.1f }, EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT);
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });
      m_final_product_plan->build();

      // create shader
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(VikingRoomUniform));
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto shader = ShaderSystem::acquire("Shaders/ObjExample/VikingRoomObjModelExample/shader");
      shader->enable_multisampling(EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT);
      shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      shader->set_vertex_layouts({ m_model_params.get_vertex_layouts() });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->build_worker();

      // create scene
      m_scene            = Scene::create();
      m_viking_room_node = Node::create();
      m_scene->get_root().add_child(m_viking_room_node);

      m_camera.set_position(glm::vec3(0.0f, 2.0f, 3.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      auto model = std::make_shared<Model>("Models/viking_room/viking_room.obj", m_model_params);
      model->m_meshes[0].m_material =
          MaterialSystem::acquire({ TextureSystem::acquire("Models/viking_room/albedo.png", {}) });

      auto viking_room = m_scene->create_entity("viking room");
      viking_room->add_component<ModelComponent>(model, shader);

      m_viking_room_node->attach_entity(viking_room);
      m_viking_room_node->rotate(glm::radians(180.f), { 0, 1, 0 });
      m_viking_room_node->rotate(glm::radians(-90.f), { 1, 0, 0 });
    }

   private:
    virtual void update(float dt) override
    {
      m_final_product_plan->begin_plan();
      {
        Scene::set_current_camera(&m_camera);
        m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

        // Update scene
        m_viking_room_node->rotate(glm::radians(dt * 3), { 0, 0, 1 });

        // Update uniform manager
        VikingRoomUniform ubo{};
        ubo.model = m_viking_room_node->get_model_mat();
        ubo.view  = m_camera.get_view();
        ubo.proj  = m_camera.get_projection();

        auto& uniform_manager = m_viking_room_node->get_entity()->get_component<ModelComponent>().get_uniform_manager();
        uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(VikingRoomUniform), &ubo);

        // Render scene
        m_scene->draw();
      }
      m_final_product_plan->end_plan();
    }
  };

} // namespace obj_example

#endif // LAYERS_VIKING_ROOM_OBJ_MODEL_EXAMPLE_LAYER_HH
