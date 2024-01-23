#ifndef LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH
#define LAYERS_BACKPACK_OBJ_MODEL_EXAMPLE_LAYER_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace obj_example
{
  // ---------- DEPTH MAP VISUALIZATION ----------
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

  static std::vector<NVertex> cube{
    // left face
    { { -1, -1, -1 }, {}, {}, { 0, 0 }, {} },
    { { -1, 1, -1 }, {}, {}, { 1, 0 }, {} },
    { { -1, 1, 1 }, {}, {}, { 1, 1 }, {} },
    { { -1, -1, 1 }, {}, {}, { 0, 1 }, {} },

    // right face
    { { 1, -1, -1 }, {}, {}, { 0, 0 }, {} },
    { { 1, 1, -1 }, {}, {}, { 1, 0 }, {} },
    { { 1, 1, 1 }, {}, {}, { 1, 1 }, {} },
    { { 1, -1, 1 }, {}, {}, { 0, 1 }, {} },

    // bottom face
    { { -1, -1, -1 }, {}, {}, { 0, 0 }, {} },
    { { 1, -1, -1 }, {}, {}, { 1, 0 }, {} },
    { { 1, -1, 1 }, {}, {}, { 1, 1 }, {} },
    { { -1, -1, 1 }, {}, {}, { 0, 1 }, {} },

    // top face
    { { -1, 1, -1 }, {}, {}, { 0, 0 }, {} },
    { { 1, 1, -1 }, {}, {}, { 1, 0 }, {} },
    { { 1, 1, 1 }, {}, {}, { 1, 1 }, {} },
    { { -1, 1, 1 }, {}, {}, { 0, 1 }, {} },

    // nose face
    { { -1, -1, -1 }, {}, {}, { 0, 0 }, {} },
    { { 1, -1, -1 }, {}, {}, { 1, 0 }, {} },
    { { 1, 1, -1 }, {}, {}, { 1, 1 }, {} },
    { { -1, 1, -1 }, {}, {}, { 0, 1 }, {} },

    // tail face
    { { -1, -1, 1 }, {}, {}, { 0, 0 }, {} },
    { { 1, -1, 1 }, {}, {}, { 1, 0 }, {} },
    { { 1, 1, 1 }, {}, {}, { 1, 1 }, {} },
    { { -1, 1, 1 }, {}, {}, { 0, 1 }, {} },
  };
  static std::vector<uint32_t> cube_idx{ 0,  2,  1,  2,  0,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
                                         12, 14, 13, 14, 12, 15, 16, 18, 17, 18, 16, 19, 20, 21, 22, 20, 22, 23 };

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

  std::vector<NVertex> floor_vertices = { { { -1, 0, -1 }, {}, {}, { 0, 0 }, {} },
                                          { { 1, 0, -1 }, {}, {}, { 1, 0 }, {} },
                                          { { 1, 0, 1 }, {}, {}, { 1, 1 }, {} },
                                          { { -1, 0, 1 }, {}, {}, { 0, 1 }, {} } };
  std::vector<uint32_t> floor_indices = { 0, 2, 1, 2, 0, 3 };

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

    float sample_offset{ 1.f };
    int calculate_lightning{ 0 };
    int calculate_shadows{ 0 };
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
#define SCENE_SIZE     20.f
#define DEPTH_MAP_SIZE 1024

   private:
    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::shared_ptr<EspBlock> m_block;
      std::shared_ptr<EspDepthBlock> m_depth_block;
      std::unique_ptr<EspRenderPlan> m_render_plan;
    } m_depth_pass;

    struct
    {
      // ---------- DEPTH MAP VISUALIZATION ----------
      // std::shared_ptr<EspShader> m_shader;
      // std::unique_ptr<EspUniformManager> m_uniform_manager;
      // std::unique_ptr<EspVertexBuffer> m_vertex_buffers_quad;
      // std::unique_ptr<EspIndexBuffer> m_index_buffer_quad;
      //

      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::shared_ptr<EspDepthBlock> m_depth_block;
      std::unique_ptr<EspRenderPlan> m_final_render_plan;
    } m_final_pass;

    std::shared_ptr<Scene> m_scene;
    Camera m_camera{};

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    } m_skybox;

    NModelParams m_model_params = { .m_position                = true,
                                    .m_normal                  = true,
                                    .m_tex_coord               = true,
                                    .m_material_texture_layout = { { 1, 0, EspTextureType::ALBEDO } } };

    struct
    {
      std::shared_ptr<Node> m_node;
      std::unique_ptr<EspUniformManager> m_depth_pass_uniform_manager;
    } m_backpack;

    struct
    {
      std::shared_ptr<Node> m_node;
      glm::mat4 m_light_space_mat{};
    } m_light;

    struct
    {
      std::shared_ptr<Node> m_node;
    } m_floor;

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
        m_depth_pass.m_shader->enable_multisampling(EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT);
        m_depth_pass.m_shader->set_vertex_layouts({ m_model_params.get_vertex_layouts() });
        m_depth_pass.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_depth_pass.m_shader->build_worker();

        m_depth_pass.m_block = EspBlock::build(EspBlockFormat::ESP_FORMAT_R8G8B8A8_UNORM,
                                               EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT,
                                               DEPTH_MAP_SIZE,
                                               DEPTH_MAP_SIZE,
                                               { 0, 0, 0 });
        m_depth_pass.m_depth_block =
            EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                 EspSampleCountFlag::ESP_SAMPLE_COUNT_4_BIT,
                                 EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                                     EspImageUsageFlag::ESP_IMAGE_USAGE_SAMPLED_BIT,
                                 DEPTH_MAP_SIZE,
                                 DEPTH_MAP_SIZE);

        m_depth_pass.m_render_plan = EspRenderPlan::create();
        m_depth_pass.m_render_plan->add_block(m_depth_pass.m_block);
        m_depth_pass.m_render_plan->add_depth_block(m_depth_pass.m_depth_block);
        m_depth_pass.m_render_plan->build();
      }

      // final pass
      {
        // ---------- DEPTH MAP VISUALIZATION ----------
        //        auto uniform_meta_data = EspUniformMetaData::create();
        //        uniform_meta_data->establish_descriptor_set();
        //        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        //
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

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(glm::mat4));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(ModelUniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(LightUniform));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(glm::vec3)); // camera pos
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4));  // light space
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE); // depth map

        m_final_pass.m_shader = ShaderSystem::acquire("Shaders/ObjExample/BackpackObjModelExample/shader_model");
        m_final_pass.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                                 EspCompareOp::ESP_COMPARE_OP_LESS);
        m_final_pass.m_shader->set_vertex_layouts({ m_model_params.get_vertex_layouts() });
        m_final_pass.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_final_pass.m_shader->build_worker();

        m_final_pass.m_uniform_manager = m_final_pass.m_shader->create_uniform_manager(2, 2);
        m_final_pass.m_uniform_manager->load_depth_block(2, 0, m_depth_pass.m_depth_block.get());
        m_final_pass.m_uniform_manager->build();

        m_final_pass.m_depth_block =
            EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                 EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                 EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

        m_final_pass.m_final_render_plan = EspRenderPlan::create_final();
        m_final_pass.m_final_render_plan->add_depth_block(std::shared_ptr{ m_final_pass.m_depth_block });
        m_final_pass.m_final_render_plan->build();
      }

      // scene initialization
      {
        m_scene = Scene::create();

        m_camera.set_position(glm::vec3(0.0f, 10.0f, 10.0f));
        m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
        m_camera.set_move_speed(3.f);
        m_camera.set_sensitivity(4.f);
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
        auto model    = std::make_shared<NModel>("Models/backpack/backpack.obj", m_model_params);
        auto backpack = m_scene->create_entity("backpack");
        backpack->add_component<NModelComponent>(model, m_final_pass.m_shader);

        m_backpack.m_node = Node::create();
        m_backpack.m_node->attach_entity(backpack);

        m_backpack.m_depth_pass_uniform_manager = m_depth_pass.m_shader->create_uniform_manager(0, 0);
        m_backpack.m_depth_pass_uniform_manager->build();
      }

      // light
      {
        auto textures = std::vector<std::shared_ptr<EspTexture>>{ TextureSystem::acquire("Textures/white.png", {}) };
        auto model    = std::make_shared<NModel>(cube, cube_idx, textures, m_model_params);
        auto light    = m_scene->create_entity("light");
        light->add_component<NModelComponent>(model, m_final_pass.m_shader);
        light->add_component<LightComponent>();

        m_light.m_node = Node::create();
        m_light.m_node->attach_entity(light);

        auto& light_component                  = light->get_component<LightComponent>();
        light_component.m_diffuse              = 1.f;
        light_component.m_specular             = 1.f;
        light_component.m_attenuation_strength = { 1.f, .09f, .032f };

        m_light.m_node->translate({ 0, 3, 3 });
        m_light.m_node->scale(.5f);
      }

      // floor
      {
        auto textures = std::vector<std::shared_ptr<EspTexture>>{ TextureSystem::acquire("Textures/floor.jpeg", {}) };
        auto model    = std::make_shared<NModel>(floor_vertices, floor_indices, textures, m_model_params);
        auto floor    = m_scene->create_entity("floor");
        floor->add_component<NModelComponent>(model, m_final_pass.m_shader);

        m_floor.m_node = Node::create();
        m_floor.m_node->attach_entity(floor);

        m_floor.m_node->translate({ 0, -2, 0 });
        m_floor.m_node->scale(SCENE_SIZE);
      }

      // attach nodes to scene
      {
        m_scene->get_root().add_child(m_backpack.m_node);
        m_scene->get_root().add_child(m_light.m_node);
        m_scene->get_root().add_child(m_floor.m_node);
      }
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      glm::vec3 light_current_pos = m_light.m_node->get_translation(ActionType::ESP_ABSOLUTE);
      glm::vec3 light_next_pos    = model_example::move_in_circle(light_current_pos, { 0, 1, 0 }, glm::radians(dt) * 2);
      m_light.m_node->set_translation(light_next_pos);

      m_depth_pass.m_render_plan->begin_plan();
      render_depth_map();
      m_depth_pass.m_render_plan->end_plan();

      m_final_pass.m_final_render_plan->begin_plan();
      m_final_pass.m_uniform_manager->attach();
      update_scene();
      render_scene();
      m_final_pass.m_final_render_plan->end_plan();
      m_final_pass.m_depth_block->clear();
    }

    void render_depth_map()
    {
      // ---------- PERSPECTIVE PROJECTION ----------
      //      glm::mat4 light_projection = glm::perspective(m_camera.get_fov(), 1.f, .1f, SCENE_SIZE);
      // ---------- ORTHOGRAPHIC PROJECTION -----------
      glm::mat4 light_projection =
          glm::ortho(-SCENE_SIZE, SCENE_SIZE, -SCENE_SIZE, SCENE_SIZE, -SCENE_SIZE, SCENE_SIZE);
      // -------------------------------------------
      glm::mat4 light_view =
          glm::lookAt(m_light.m_node->get_translation(ActionType::ESP_ABSOLUTE), { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
      m_light.m_light_space_mat = light_projection * light_view;

      auto viewport        = EspViewport();
      viewport.m_height    = DEPTH_MAP_SIZE;
      viewport.m_width     = DEPTH_MAP_SIZE;
      viewport.m_min_depth = 0;
      viewport.m_max_depth = 1;

      EspScissorRect scissor_rect = {};
      scissor_rect.m_width        = DEPTH_MAP_SIZE;
      scissor_rect.m_height       = DEPTH_MAP_SIZE;

      m_depth_pass.m_shader->only_attach();
      m_depth_pass.m_shader->set_scissors(scissor_rect);
      m_depth_pass.m_shader->set_viewport(viewport);

      // render backpack
      {
        glm::mat4 model_mat = m_backpack.m_node->get_model_mat(ActionType::ESP_ABSOLUTE);

        m_backpack.m_depth_pass_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &model_mat);
        m_backpack.m_depth_pass_uniform_manager->update_buffer_uniform(0,
                                                                       0,
                                                                       sizeof(glm::mat4),
                                                                       sizeof(glm::mat4),
                                                                       &m_light.m_light_space_mat);
        m_backpack.m_depth_pass_uniform_manager->attach();

        auto& model = m_backpack.m_node->get_entity()->get_component<NModelComponent>().get_model();
        for (auto model_node : model)
        {
          for (auto& mesh_idx : model_node.m_current_node->m_meshes)
          {
            auto& mesh = model.m_meshes[mesh_idx];
            esp::EspJob::draw_indexed(mesh.m_index_count, 1, mesh.m_first_index);
          }
        }
      }
    }

    void update_scene()
    {
      auto& light = m_light.m_node->get_entity()->get_component<LightComponent>();
      LightUniform l_ubo{};
      l_ubo.m_position             = m_light.m_node->get_translation(ActionType::ESP_ABSOLUTE);
      l_ubo.m_diffuse              = glm::vec3(light.m_diffuse);
      l_ubo.m_specular             = glm::vec3(light.m_specular);
      l_ubo.m_attenuation_strength = light.m_attenuation_strength;

      auto camera_pos = m_camera.get_position();

      // update backpack
      {
        ModelUniform ubo{};
        ubo.model               = m_backpack.m_node->get_model_mat(ActionType::ESP_ABSOLUTE);
        ubo.view                = m_camera.get_view();
        ubo.projection          = m_camera.get_projection();
        ubo.calculate_lightning = 1;

        auto& uniform_manager = m_backpack.m_node->get_entity()->get_component<NModelComponent>().get_uniform_manager();
        uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(ModelUniform), &ubo);
        uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(LightUniform), &l_ubo);
        uniform_manager.update_buffer_uniform(0, 2, 0, sizeof(glm::vec3), &camera_pos);
        uniform_manager.update_buffer_uniform(0, 3, 0, sizeof(glm::mat4), &m_light.m_light_space_mat);
      }

      // update light
      {
        ModelUniform ubo{};
        ubo.model      = m_light.m_node->get_model_mat(ActionType::ESP_ABSOLUTE);
        ubo.view       = m_camera.get_view();
        ubo.projection = m_camera.get_projection();

        auto& uniform_manager = m_light.m_node->get_entity()->get_component<NModelComponent>().get_uniform_manager();
        uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(ModelUniform), &ubo);
        uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(LightUniform), &l_ubo);
        uniform_manager.update_buffer_uniform(0, 2, 0, sizeof(glm::vec3), &camera_pos);
        uniform_manager.update_buffer_uniform(0, 3, 0, sizeof(glm::mat4), &m_light.m_light_space_mat);
      }

      // update floor
      {
        ModelUniform ubo{};
        ubo.model               = m_floor.m_node->get_model_mat(ActionType::ESP_ABSOLUTE);
        ubo.view                = m_camera.get_view();
        ubo.projection          = m_camera.get_projection();
        ubo.sample_offset       = SCENE_SIZE;
        ubo.calculate_lightning = 1;
        ubo.calculate_shadows   = 1;

        auto& uniform_manager = m_floor.m_node->get_entity()->get_component<NModelComponent>().get_uniform_manager();
        uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(ModelUniform), &ubo);
        uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(LightUniform), &l_ubo);
        uniform_manager.update_buffer_uniform(0, 2, 0, sizeof(glm::vec3), &camera_pos);
        uniform_manager.update_buffer_uniform(0, 3, 0, sizeof(glm::mat4), &m_light.m_light_space_mat);
      }
    }

    void render_scene()
    {
      // render scene
      m_scene->draw();

      // render skybox
      SkyboxUniform ubo{};
      ubo.view       = glm::mat4(glm::mat3(m_camera.get_view()));
      ubo.projection = m_camera.get_projection();

      m_skybox.m_shader->attach();
      m_skybox.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(SkyboxUniform), &ubo);
      m_skybox.m_uniform_manager->attach();

      m_skybox.m_vertex_buffer->attach();
      EspJob::draw(skybox_vertices.size());

      // ---------- DEPTH MAP VISUALIZATION ----------
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
