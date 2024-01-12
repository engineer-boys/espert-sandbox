#ifndef LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace obj_example
{
  //  struct QuatVertex
  //  {
  //    glm::vec2 pos;
  //    glm::vec2 texCoord;
  //
  //    QuatVertex(glm::vec2 pos, glm::vec2 texCoord) : pos{ pos }, texCoord{ texCoord } {}
  //  };
  //
  //  static std::vector<QuatVertex> quad{ { { -1, -1 }, { 0, 0 } },
  //                                       { { 1, -1 }, { 1, 0 } },
  //                                       { { 1, 1 }, { 1, 1 } },
  //                                       { { -1, 1 }, { 0, 1 } } };
  //
  //  static std::vector<uint32_t> quad_idx{ 0, 1, 2, 2, 3, 0 };

  std::vector<glm::vec3> skybox_vertices = {
    // positions
    { -1.0f, 1.0f, -1.0f },  { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },  { 1.0f, 1.0f, -1.0f },   { -1.0f, 1.0f, -1.0f },

    { -1.0f, -1.0f, 1.0f },  { -1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f },
    { -1.0f, 1.0f, -1.0f },  { -1.0f, 1.0f, 1.0f },   { -1.0f, -1.0f, 1.0f },

    { 1.0f, -1.0f, -1.0f },  { 1.0f, -1.0f, 1.0f },   { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },    { 1.0f, 1.0f, -1.0f },   { 1.0f, -1.0f, -1.0f },

    { -1.0f, -1.0f, 1.0f },  { -1.0f, 1.0f, 1.0f },   { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },    { 1.0f, -1.0f, 1.0f },   { -1.0f, -1.0f, 1.0f },

    { -1.0f, 1.0f, -1.0f },  { 1.0f, 1.0f, -1.0f },   { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },    { -1.0f, 1.0f, 1.0f },   { -1.0f, 1.0f, -1.0f },

    { -1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, 1.0f },  { 1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },  { -1.0f, -1.0f, 1.0f },  { 1.0f, -1.0f, 1.0f }
  };

  std::vector<Mesh::Vertex> floor_vertices = { { { -1, 0, -1 }, {}, {}, { 0, 0 }, {} },
                                               { { 1, 0, -1 }, {}, {}, { 1, 0 }, {} },
                                               { { 1, 0, 1 }, {}, {}, { 1, 1 }, {} },
                                               { { -1, 0, 1 }, {}, {}, { 0, 1 }, {} } };
  std::vector<uint32_t> floor_indices      = { 0, 2, 1, 2, 0, 3 };

  struct SkyboxUniform
  {
    glm::mat4 view;
    glm::mat4 projection;
  };

  struct ModelUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
  };

  struct LightUniform
  {
    glm::vec3 m_position;

    alignas(16) glm::vec3 m_diffuse;
    alignas(16) glm::vec3 m_specular;

    alignas(16) glm::vec3 m_attenuation_strength;
  };

  struct LightComponent
  {
    float m_diffuse; // color
    float m_specular;

    glm::vec3 m_attenuation_strength; // (constant, linear, quadratic)
  };

  class BackpackObjModelExampleLayer : public Layer
  {
   private:
    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspRenderPlan> m_render_plan;
      std::shared_ptr<EspBlock> m_block; // TODO: remove this block and render only to depth block
      std::shared_ptr<EspDepthBlock> m_depth_block;
    } m_depth_pass;

    struct
    {
      //      std::shared_ptr<EspShader> m_shader;
      //      std::unique_ptr<EspUniformManager> m_uniform_manager;
      //      std::unique_ptr<EspVertexBuffer> m_vertex_buffers_quad;
      //      std::unique_ptr<EspIndexBuffer> m_index_buffer_quad;
      std::unique_ptr<EspRenderPlan> m_final_render_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
    } m_final_pass;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    } m_skybox;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspUniformManager> m_depth_uniform_manager;
      std::shared_ptr<Model> m_model;
    } m_backpack;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::shared_ptr<Model> m_model;
    } m_light;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspUniformManager> m_depth_uniform_manager;
      std::shared_ptr<Model> m_model;
    } m_floor;

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Node> m_backpack_node;
    std::shared_ptr<Node> m_light_node;
    std::shared_ptr<Node> m_floor_node;
    Camera m_camera{};

   public:
    BackpackObjModelExampleLayer()
    {
      // depth pass
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4) * 2);

        m_depth_pass.m_shader = ShaderSystem::acquire("Shaders/ObjExample/BackpackObjModelExample/shader_depth");
        m_depth_pass.m_shader->set_attachment_formats({ EspBlockFormat::ESP_FORMAT_R8G8B8A8_UNORM });
        m_depth_pass.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                                 EspCompareOp::ESP_COMPARE_OP_LESS);
        m_depth_pass.m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        m_depth_pass.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_depth_pass.m_shader->build_worker();

        m_depth_pass.m_block       = EspBlock::build(EspBlockFormat::ESP_FORMAT_R8G8B8A8_UNORM,
                                               EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                                     { 0, 0, 0 });
        m_depth_pass.m_depth_block = EspDepthBlock::build(
            EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
            EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
            EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                EspImageUsageFlag::ESP_IMAGE_USAGE_SAMPLED_BIT); // TODO: lower depth map resolution, add custom
                                                                 // viewport to match depth map resolution

        m_depth_pass.m_render_plan = EspRenderPlan::create();
        m_depth_pass.m_render_plan->add_block(m_depth_pass.m_block);
        m_depth_pass.m_render_plan->add_depth_block(m_depth_pass.m_depth_block);
        m_depth_pass.m_render_plan->build();
      }

      // final pass
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        //        m_final_pass.m_shader = ShaderSystem::acquire("Shaders/OffscreenRnd/shader_on");
        //        m_final_pass.m_shader->set_vertex_layouts(
        //            { VTX_LAYOUT(sizeof(QuatVertex),
        //                         0,
        //                         ESP_VERTEX_INPUT_RATE_VERTEX,
        //                         ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex, pos)),
        //                         ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(QuatVertex, texCoord))) });
        //        m_final_pass.m_shader->set_worker_layout(std::move(uniform_meta_data));
        //        m_final_pass.m_shader->build_worker();
        //
        //        m_final_pass.m_uniform_manager = m_final_pass.m_shader->create_uniform_manager();
        //        m_final_pass.m_uniform_manager->load_depth_block(0, 0, m_depth_pass.m_depth_block.get());
        //        m_final_pass.m_uniform_manager->build();
        //
        //        m_final_pass.m_vertex_buffers_quad = EspVertexBuffer::create(quad.data(), sizeof(QuatVertex),
        //        quad.size()); m_final_pass.m_index_buffer_quad   = EspIndexBuffer::create(quad_idx.data(),
        //        quad_idx.size());

        m_final_pass.m_depth_block =
            EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                 EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                 EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

        m_final_pass.m_final_render_plan = EspRenderPlan::create_final();
        m_final_pass.m_final_render_plan->add_depth_block(std::shared_ptr{ m_final_pass.m_depth_block });
        m_final_pass.m_final_render_plan->build();
      }

      // skybox
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(SkyboxUniform));
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_skybox.m_shader = ShaderSystem::acquire("Shaders/ObjExample/BackpackObjModelExample/shader_skybox");
        m_skybox.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
        m_skybox.m_shader->set_vertex_layouts({ VTX_LAYOUT(sizeof(glm::vec3),
                                                           0,
                                                           ESP_VERTEX_INPUT_RATE_VERTEX,
                                                           ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, 0), ) });
        m_skybox.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_skybox.m_shader->build_worker();

        m_skybox.m_uniform_manager = m_skybox.m_shader->create_uniform_manager();
        m_skybox.m_uniform_manager->load_texture(0, 1, TextureSystem::acquire_cubemap("Textures/skybox/sk.jpg"));
        m_skybox.m_uniform_manager->build();

        m_skybox.m_vertex_buffer =
            EspVertexBuffer::create(skybox_vertices.data(), sizeof(glm::vec3), skybox_vertices.size());
      }

      // backpack
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(ModelUniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(LightUniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(glm::vec3));
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_backpack.m_shader = ShaderSystem::acquire("Shaders/ObjExample/BackpackObjModelExample/shader_backpack");
        m_backpack.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                               EspCompareOp::ESP_COMPARE_OP_LESS);
        m_backpack.m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        m_backpack.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_backpack.m_shader->build_worker();

        m_backpack.m_uniform_manager = m_backpack.m_shader->create_uniform_manager(0, 0);
        m_backpack.m_uniform_manager->build();

        m_backpack.m_depth_uniform_manager = m_depth_pass.m_shader->create_uniform_manager(0, 0);
        m_backpack.m_depth_uniform_manager->build();

        Model::Builder model_builder{};
        model_builder.set_shader(m_backpack.m_shader);
        model_builder.load_model("Models/backpack/backpack.obj");
        m_backpack.m_model = std::make_shared<Model>(model_builder);
      }

      // light
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(ModelUniform));

        m_light.m_shader = ShaderSystem::acquire("Shaders/ObjExample/BackpackObjModelExample/shader_light");
        m_light.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                            EspCompareOp::ESP_COMPARE_OP_LESS);
        m_light.m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        m_light.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_light.m_shader->build_worker();

        m_light.m_uniform_manager = m_light.m_shader->create_uniform_manager(0, 0);
        m_light.m_uniform_manager->build();

        auto cube_mesh  = std::make_shared<Mesh>(model_example::create_cube_vertices({ 1.f, 1.f, 1.f }));
        m_light.m_model = std::make_shared<Model>(cube_mesh);
      }

      // floor
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(ModelUniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(LightUniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(glm::vec3));
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_floor.m_shader = ShaderSystem::acquire("Shaders/ObjExample/BackpackObjModelExample/shader_floor");
        m_floor.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                            EspCompareOp::ESP_COMPARE_OP_LESS);
        m_floor.m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        m_floor.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_floor.m_shader->build_worker();

        m_floor.m_uniform_manager = m_floor.m_shader->create_uniform_manager(0, 0);
        m_floor.m_uniform_manager->build();

        m_floor.m_depth_uniform_manager = m_depth_pass.m_shader->create_uniform_manager(0, 0);
        m_floor.m_depth_uniform_manager->build();

        auto floor_albedo_texture = TextureSystem::acquire("Textures/floor.jpeg", {});
        auto floor_material       = MaterialSystem::acquire("floor_material",
                                                            { floor_albedo_texture },
                                                      m_floor.m_shader,
                                                            { { 1, 0, EspTextureType::ALBEDO } });

        auto floor_mesh = std::make_shared<Mesh>(floor_vertices, floor_indices, floor_material);
        m_floor.m_model = std::make_shared<Model>(floor_mesh);
      }

      // scene
      {
        m_scene         = Scene::create();
        m_backpack_node = Node::create();
        m_light_node    = Node::create();
        m_floor_node    = Node::create();
        m_scene->get_root().add_child(m_backpack_node);
        m_scene->get_root().add_child(m_light_node);
        m_scene->get_root().add_child(m_floor_node);

        m_camera.set_position(glm::vec3(-2.0f, 1.0f, 5.0f));
        m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
        m_camera.set_move_speed(3.f);
        m_camera.set_sensitivity(4.f);

        auto backpack = m_scene->create_entity("backpack");
        backpack->add_component<TransformComponent>();
        backpack->add_component<ModelComponent>(m_backpack.m_model);
        m_backpack_node->attach_entity(backpack);

        auto light = m_scene->create_entity("light");
        light->add_component<TransformComponent>();
        light->add_component<ModelComponent>(m_light.m_model);
        light->add_component<LightComponent>();
        m_light_node->attach_entity(light);

        auto& light_component                  = light->get_component<LightComponent>();
        light_component.m_diffuse              = 1.f;
        light_component.m_specular             = 1.f;
        light_component.m_attenuation_strength = { 1.f, .09f, .032f };

        action::TransformAction::set_translation(m_light_node.get(), { 2.f, 1.f, 2.f }, action::ABSOLUTE);
        action::TransformAction::set_scale(m_light_node.get(), .5f, action::ABSOLUTE);

        auto floor = m_scene->create_entity("floor");
        floor->add_component<TransformComponent>();
        floor->add_component<ModelComponent>(m_floor.m_model);
        m_floor_node->attach_entity(floor);

        action::TransformAction::set_translation(m_floor_node.get(), { 0.f, -2.f, 0.f }, action::ABSOLUTE);
        action::TransformAction::set_scale(m_floor_node.get(), 10.f, action::ABSOLUTE);
      }
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_depth_pass.m_render_plan->begin_plan();
      render_depth_map();
      m_depth_pass.m_render_plan->end_plan();

      m_final_pass.m_final_render_plan->begin_plan();
      render_scene();
      m_final_pass.m_final_render_plan->end_plan();

      m_final_pass.m_depth_block->clear();
    }

    void render_depth_map()
    {
      auto light_transform = m_light_node->get_entity()->get_component<TransformComponent>();

      glm::mat4 light_projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, -10.f, 10.f);
      glm::mat4 light_view       = glm::lookAt(light_transform.get_translation(), { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
      glm::mat4 light_space_mat  = light_projection * light_view;

      m_depth_pass.m_shader->attach();

      // render backpack
      {
        auto transform = m_backpack_node->get_entity()->get_component<TransformComponent>();
        auto model_mat = transform.get_model_mat();

        m_backpack.m_depth_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &model_mat);
        m_backpack.m_depth_uniform_manager->update_buffer_uniform(0,
                                                                  0,
                                                                  sizeof(glm::mat4),
                                                                  sizeof(glm::mat4),
                                                                  &light_space_mat);
        m_backpack.m_depth_uniform_manager->attach();
        m_backpack.m_model->draw_raw();
      }

      // render floor
      {
        auto transform = m_floor_node->get_entity()->get_component<TransformComponent>();
        auto model_mat = transform.get_model_mat();

        m_floor.m_depth_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &model_mat);
        m_floor.m_depth_uniform_manager->update_buffer_uniform(0,
                                                               0,
                                                               sizeof(glm::mat4),
                                                               sizeof(glm::mat4),
                                                               &light_space_mat);
        m_floor.m_depth_uniform_manager->attach();
        m_floor.m_model->draw_raw();
      }
    }

    void render_scene()
    {
      auto light_transform = m_light_node->get_entity()->get_component<TransformComponent>();
      auto& light          = m_light_node->get_entity()->get_component<LightComponent>();
      LightUniform l_ubo{};
      l_ubo.m_position             = light_transform.get_translation();
      l_ubo.m_diffuse              = glm::vec3(light.m_diffuse);
      l_ubo.m_specular             = glm::vec3(light.m_specular);
      l_ubo.m_attenuation_strength = light.m_attenuation_strength;

      auto camera_pos = m_camera.get_positiion();

      // render backpack
      {
        auto backpack_transform = m_backpack_node->get_entity()->get_component<TransformComponent>();
        ModelUniform ubo{};
        ubo.model      = backpack_transform.get_model_mat();
        ubo.view       = m_camera.get_view();
        ubo.projection = m_camera.get_projection();

        m_backpack.m_shader->attach();
        m_backpack.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(ModelUniform), &ubo);
        m_backpack.m_uniform_manager->update_buffer_uniform(0, 1, 0, sizeof(LightUniform), &l_ubo);
        m_backpack.m_uniform_manager->update_buffer_uniform(0, 2, 0, sizeof(glm::vec3), &camera_pos);
        m_backpack.m_uniform_manager->attach();
        m_backpack.m_model->draw();
      }

      // render light
      {
        auto transform = m_light_node->get_entity()->get_component<TransformComponent>();

        ModelUniform ubo{};
        ubo.model      = transform.get_model_mat();
        ubo.view       = m_camera.get_view();
        ubo.projection = m_camera.get_projection();

        m_light.m_shader->attach();
        m_light.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(ModelUniform), &ubo);
        m_light.m_uniform_manager->attach();
        m_light.m_model->draw();
      }

      // render floor
      {
        auto transform = m_floor_node->get_entity()->get_component<TransformComponent>();

        ModelUniform ubo{};
        ubo.model      = transform.get_model_mat();
        ubo.view       = m_camera.get_view();
        ubo.projection = m_camera.get_projection();

        m_floor.m_shader->attach();
        m_floor.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(ModelUniform), &ubo);
        m_floor.m_uniform_manager->update_buffer_uniform(0, 1, 0, sizeof(LightUniform), &l_ubo);
        m_floor.m_uniform_manager->update_buffer_uniform(0, 2, 0, sizeof(glm::vec3), &camera_pos);
        m_floor.m_uniform_manager->attach();
        m_floor.m_model->draw();
      }

      // render skybox
      {
        SkyboxUniform ubo{};
        ubo.view       = glm::mat4(glm::mat3(m_camera.get_view()));
        ubo.projection = m_camera.get_projection();

        m_skybox.m_shader->attach();
        m_skybox.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(SkyboxUniform), &ubo);
        m_skybox.m_uniform_manager->attach();

        m_skybox.m_vertex_buffer->attach();
        EspJob::draw(skybox_vertices.size());
      }
      //      m_final_pass.m_shader->attach();
      //      m_final_pass.m_vertex_buffers_quad->attach();
      //
      //      m_final_pass.m_uniform_manager->attach();
      //
      //      m_final_pass.m_index_buffer_quad->attach();
      //      EspJob::draw_indexed(quad_idx.size());
    }
  };
} // namespace obj_example

#endif // LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH
