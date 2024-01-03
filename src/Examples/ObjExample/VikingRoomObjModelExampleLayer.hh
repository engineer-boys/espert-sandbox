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
    std::shared_ptr<EspShader> m_shader;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::shared_ptr<Scene> m_scene;
    Camera m_camera{};

    std::shared_ptr<Node> m_viking_room_node;

    std::shared_ptr<EspDepthBlock> m_depth_block;

    std::unique_ptr<EspRenderPlan> m_final_product_plan;

   public:
    VikingRoomObjModelExampleLayer()
    {
      m_scene            = Scene::create();
      m_viking_room_node = Node::create();
      m_scene->get_root().add_child(m_viking_room_node);

      m_camera.set_position(glm::vec3(0.0f, -2.0f, 3.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      m_depth_block =
          EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT);

      m_final_product_plan = EspRenderPlan::build_final(EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT);
      m_final_product_plan->add_depth_block(std::shared_ptr{ m_depth_block });

      m_shader = ShaderSystem::acquire(
          "Shaders/ObjExample/VikingRoomObjModelExample/shader",
          { .depthtest_config     = { .enable     = true,
                                      .format     = EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                      .compare_op = EspCompareOp::ESP_COMPARE_OP_LESS },
            .multisampling_config = { .enable            = true,
                                      .sample_count_flag = EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT } });

      m_uniform_manager = m_shader->create_uniform_manager(0, 0);
      m_uniform_manager->build();

      auto mesh = Model::Builder{}
                      .set_shader(m_shader)
                      .load_model("Models/viking_room/viking_room.obj",
                                  { .p_flags = EspProcessFlipUVs, .load_material = false })
                      .m_meshes[0];
      auto material =
          MaterialSystem::acquire({ TextureSystem::acquire("Models/viking_room/albedo.png", {}) }, m_shader);
      mesh->set_material(material);

      auto viking_room = m_scene->create_entity("viking room");
      viking_room->add_component<TransformComponent>();
      viking_room->add_component<ModelComponent>(std::make_shared<Model>(mesh));

      m_viking_room_node->attach_entity(viking_room);
      TransformAction::update_rotation(m_viking_room_node.get(), glm::radians(90.f), { 1.f, 0.f, 0.f }, ABSOLUTE);
    }

   private:
    virtual void update(float dt) override
    {
      m_final_product_plan->begin_plan();
      {
        Scene::set_current_camera(&m_camera);

        m_shader->attach();

        m_viking_room_node->act(
            [&dt](Node* node)
            {
              auto& transform = node->get_entity()->get_component<TransformComponent>();
              transform.reset();
              TransformAction::update_rotation(node, dt / 5, { 0.f, 0.f, 1.f }, ABSOLUTE);
            });

        m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

        auto& transform = m_viking_room_node->get_entity()->get_component<TransformComponent>();
        VikingRoomUniform ubo{};
        ubo.model = transform.get_model_mat();
        ubo.view  = m_camera.get_view();
        ubo.proj  = m_camera.get_projection();
        m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(VikingRoomUniform), &ubo);
        m_uniform_manager->attach();

        auto& model = m_viking_room_node->get_entity()->get_component<ModelComponent>();
        model.m_model_handle->draw();
      }
      m_final_product_plan->end_plan();
    }
  };

} // namespace obj_example

#endif // LAYERS_VIKING_ROOM_OBJ_MODEL_EXAMPLE_LAYER_HH
