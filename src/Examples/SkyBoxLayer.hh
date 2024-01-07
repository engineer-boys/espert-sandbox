#ifndef LAYERS_SKY_BOX_LAYER_HH
#define LAYERS_SKY_BOX_LAYER_HH

// libs
#include "Espert.hh"
#include "ModelExample/ModelExampleUtils.hh"

using namespace esp;

namespace advance_rendering_example
{
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

  struct MvpSkyBoxUniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  struct VpSkyBoxUniform
  {
    glm::mat4 view;
    glm::mat4 proj;
  };

  class SkyBoxLayer : public Layer
  {
   private:
    Camera m_camera{};

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
      std::shared_ptr<Model> m_model;
    } m_sphere;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    } m_skybox;

    struct
    {
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
    } m_final_pass;

   public:
    SkyBoxLayer()
    {
      m_camera.set_position(glm::vec3(0.0f, 0.0f, 3.0f));
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      // final pass
      {
        m_final_pass.m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                                          EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);

        m_final_pass.m_final_product_plan = EspRenderPlan::create_final();
        m_final_pass.m_final_product_plan->add_depth_block(std::shared_ptr{ m_final_pass.m_depth_block });
        m_final_pass.m_final_product_plan->build();
      }

      // skybox
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(VpSkyBoxUniform));
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_skybox.m_shader = ShaderSystem::acquire("Shaders/SkyBoxExample/skybox");
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

      // model
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(MvpSkyBoxUniform));

        m_sphere.m_shader = ShaderSystem::acquire("Shaders/SkyBoxExample/shader_f");
        m_sphere.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
        m_sphere.m_shader->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        m_sphere.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_sphere.m_shader->build_worker();

        Model::Builder model_builder{};
        model_builder.set_shader(m_sphere.m_shader);
        model_builder.load_model("Models/sphere/sphere.gltf", { .layouts = {} });
        m_sphere.m_model = std::make_shared<Model>(model_builder);

        m_sphere.m_uniform_manager = m_sphere.m_shader->create_uniform_manager();
        m_sphere.m_uniform_manager->build();
      }
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_final_pass.m_final_product_plan->begin_plan();
      {
        m_sphere.m_shader->attach();

        MvpSkyBoxUniform ubo{};
        ubo.model = glm::mat4(1);
        ubo.model = glm::translate(ubo.model, glm::vec3{ 0, 0, -3.0f });
        ubo.view  = m_camera.get_view();
        ubo.proj  = m_camera.get_projection();
        m_sphere.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(MvpSkyBoxUniform), &ubo);
        m_sphere.m_uniform_manager->attach();

        m_sphere.m_model->draw();

        m_skybox.m_shader->attach();
        m_skybox.m_vertex_buffer->attach();

        VpSkyBoxUniform vp{};
        vp.view = glm::mat4(glm::mat3(m_camera.get_view()));
        vp.proj = m_camera.get_projection();
        m_skybox.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(VpSkyBoxUniform), &vp);
        m_skybox.m_uniform_manager->attach();

        EspJob::draw(skybox_vertices.size());
      }
      m_final_pass.m_final_product_plan->end_plan();

      m_final_pass.m_depth_block->clear();
    }
  };
} // namespace advance_rendering_example

#endif // LAYERS_SKY_BOX_LAYER_HH
