#ifndef LAYERS_PBR_BASE_LAYER_2_HH
#define LAYERS_PBR_BASE_LAYER_2_HH

// libs
#include "../ModelExample/ModelExampleUtils.hh"
#include "Espert.hh"

using namespace esp;

#define GRID_DIM 7

namespace advance_rendering_example
{
  struct MVP_PBR_Uniform2
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    alignas(16) glm::vec3 camPos;
  };

  struct Material2_PBR_Uniform
  {
    // Parameter block used as push constant block
    struct PushBlock
    {
      float roughness;
      float metallic;
      float r, g, b;
    } params;
    std::string name;

    Material2_PBR_Uniform(){};

    Material2_PBR_Uniform(std::string n, glm::vec3 c, float r, float m) : name(n)
    {
      params.roughness = r;
      params.metallic  = m;
      params.r         = c.r;
      params.g         = c.g;
      params.b         = c.b;
    };
  };

  //   struct Light_PBR_Uniform
  //   {
  //     alignas(16) glm::vec3 position;
  //     alignas(16) glm::vec3 color;

  //     Light_PBR_Uniform(glm::vec3 position, glm::vec3 color) : position{ position }, color{ color } {}
  //   };

  class PBRBaseLayer2 : public Layer
  {
   private:
    Camera m_camera{};
    std::shared_ptr<Mesh> m_mesh;
    Material2_PBR_Uniform m_mat;

    //   std::shared_ptr<EspDepthBlock> m_depth_block;

    struct
    {
      std::unique_ptr<EspWorker> m_pipeline;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
    } m_final_pass;

   public:
    PBRBaseLayer2()
    {
      m_camera.set_position(glm::vec3(10.0f, -13.0f, 1.8f));
      m_camera.set_fov(60.0f);
      m_camera.set_near(0.1f);
      m_camera.set_far(256.0f);
      m_camera.look_at(glm::vec3(0.0f, 0.0f, 0.0f));
      m_camera.set_move_speed(3.f);
      m_camera.set_sensitivity(4.f);

      // m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
      //                                                   EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT);
      // builder->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);

      // final pass
      {
        m_final_pass.m_final_product_plan = EspRenderPlan::build_final();

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(MVP_PBR_Uniform2));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, sizeof(glm::vec4) * 4);
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_VTX_STAGE, 0, sizeof(glm::vec3));
        uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_FRAG_STAGE,
                                            sizeof(glm::vec3),
                                            sizeof(Material2_PBR_Uniform::PushBlock));

        auto builder = EspWorkerBuilder::create();
        builder->set_shaders("../resources/Shaders/PBRBaseExample/shader_f.vert.spv",
                             "../resources/Shaders/PBRBaseExample/shader_f.frag.spv");
        builder->set_vertex_layouts({ Mesh::Vertex::get_vertex_layout() });
        builder->set_pipeline_layout(std::move(uniform_meta_data));
        m_final_pass.m_pipeline = builder->build_worker();

        m_final_pass.m_uniform_manager = m_final_pass.m_pipeline->create_uniform_manager();
        m_final_pass.m_uniform_manager->build();
      }

      m_mesh = std::shared_ptr<Mesh>{
        Model::Builder{}.load_model("Models/sphere/sphere.gltf", Model::EspProcessFlipUVs).m_meshes[0]
      };

      const float p = 15.0f;
      std::vector<glm::vec4> lights{};
      lights.push_back({ -p, -p * 0.5f, -p, 1.0f });
      lights.push_back({ -p, -p * 0.5f, p, 1.0f });
      lights.push_back({ p, -p * 0.5f, p, 1.0f });
      lights.push_back({ p, -p * 0.5f, -p, 1.0f });
      m_final_pass.m_uniform_manager->set_buffer_uniform(0, 1, 0, sizeof(glm::vec4) * 4, lights.data());

      m_mat = Material2_PBR_Uniform("Gold", glm::vec3(1.0f, 0.765557f, 0.336057f), 0.1f, 1.0f);
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());

      m_final_pass.m_final_product_plan->begin_plan();
      {
        m_final_pass.m_pipeline->attach();
        MVP_PBR_Uniform2 ubo{};
        ubo.model  = glm::mat4(1);
        ubo.view   = m_camera.get_view();
        ubo.proj   = m_camera.get_projection();
        ubo.camPos = m_camera.get_positiion();
        m_final_pass.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(MVP_PBR_Uniform2), &ubo);
        m_final_pass.m_uniform_manager->attach();

        for (uint32_t y = 0; y < GRID_DIM; y++)
        {
          for (uint32_t x = 0; x < GRID_DIM; x++)
          {
            glm::vec3 pos = glm::vec3(float(x - (GRID_DIM / 2.0f)) * 2.5f, 0.0f, float(y - (GRID_DIM / 2.0f)) * 2.5f);
            m_final_pass.m_uniform_manager->update_push_uniform(0, &pos);

            Material2_PBR_Uniform mat = m_mat;
            mat.params.metallic       = glm::clamp((float)x / (float)(GRID_DIM - 1), 0.1f, 1.0f);
            mat.params.roughness      = glm::clamp((float)y / (float)(GRID_DIM - 1), 0.05f, 1.0f);
            m_final_pass.m_uniform_manager->update_push_uniform(1, &(mat.params));

            m_mesh->draw();
          }
        }
      }
      m_final_pass.m_final_product_plan->end_plan();
    }
  };
} // namespace advance_rendering_example

#endif // LAYERS_PBR_BASE_LAYER_2_HH
