#ifndef ESPERT_SANDBOX_TORUSLAYER_HH
#define ESPERT_SANDBOX_TORUSLAYER_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  class Torus
  {
   private:
    float m_r;
    float m_R;
    float m_theta;
    float m_phi;

    float m_density;
  };

  static std::vector<Vertex> generate_torus_vertices(float R, float r, int num_segments_theta, int num_segments_phi);
  static std::vector<uint32_t> generate_torus_indices(int num_segments_theta, int num_segments_phi);

  class TorusLayer : public Layer
  {
   private:
    std::shared_ptr<EspDepthBlock> m_depth_block;
    std::unique_ptr<EspRenderPlan> m_final_render_plan;

    ModelParams m_model_params = { .m_position = true };

    std::shared_ptr<Scene> m_scene;

    struct
    {
      std::shared_ptr<Model> m_model;
      std::shared_ptr<Node> m_node;
    } m_torus;

   public:
    TorusLayer()
    {
      // create render plan
      m_depth_block = EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                           EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                           EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

      m_final_render_plan = EspRenderPlan::create_final();
      m_final_render_plan->add_depth_block(std::shared_ptr{ m_depth_block });
      m_final_render_plan->build();

      // create shader
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4));

      auto shader = ShaderSystem::acquire("Shaders/MG1/shader");
      shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      shader->set_vertex_layouts({ m_model_params.get_vertex_layouts() });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->set_rasterizer_settings({ .m_polygon_mode = ESP_POLYGON_MODE_LINE, .m_cull_mode = ESP_CULL_MODE_NONE });
      shader->build_worker();

      // create scene
      m_scene        = Scene::create();
      m_torus.m_node = Node::create();
      m_scene->get_root().add_child(m_torus.m_node);
      m_scene->add_camera(std::make_shared<Camera>());

      auto camera = m_scene->get_camera(0);
      camera->set_position(glm::vec3{ 0.f, 1.f, 5.f });
      camera->look_at(glm::vec3{ 0.f, 0.f, 0.f });
      camera->set_move_speed(3.f);
      camera->set_sensitivity(4.f);
      camera->set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());
      m_scene->set_current_camera(camera.get());

      float R                = 1.f;
      float r                = .3f;
      int num_segments_theta = 30; // ile okręgów
      int num_segments_phi   = 20; // ile punktów na okręgu

      auto vertices = generate_torus_vertices(R, r, num_segments_theta, num_segments_phi);
      auto indices  = generate_torus_indices(num_segments_theta, num_segments_phi);

      m_torus.m_model =
          std::make_shared<Model>(vertices, indices, std::vector<std::shared_ptr<EspTexture>>{}, m_model_params);

      auto entity = m_scene->create_entity();
      entity->add_component<ModelComponent>(m_torus.m_model, shader);

      m_torus.m_node->attach_entity(entity);
    }

   private:
    virtual void update(float dt) override
    {
      auto camera = Scene::get_current_camera();

      m_torus.m_node->rotate(dt / 2, { 0, 1, 0 });
      m_torus.m_node->rotate(dt / 4, { 1, 0, 0 });

      auto& uniform_manager = m_torus.m_node->get_entity()->get_component<ModelComponent>().get_uniform_manager();
      glm::mat4 mvp         = camera->get_projection() * camera->get_view() * m_torus.m_node->get_model_mat();
      uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &mvp);

      m_final_render_plan->begin_plan();
      {
        m_scene->draw();
      }
      m_final_render_plan->end_plan();
    }
  };

  static std::vector<Vertex> generate_torus_vertices(float R, float r, int num_segments_theta, int num_segments_phi)
  {
    std::vector<Vertex> vertices{};

    float d_theta = 2.0f * M_PI / num_segments_theta;
    float d_phi   = 2.0f * M_PI / num_segments_phi;

    for (int i = 0; i < num_segments_theta; i++)
    {
      float theta = i * d_theta;

      for (int j = 0; j <= num_segments_phi; j++)
      {
        float phi = j * d_phi;

        float x = (R + r * std::cos(phi)) * std::cos(theta);
        float y = (R + r * std::cos(phi)) * std::sin(theta);
        float z = r * std::sin(phi);

        vertices.push_back({ { x, y, z } });
      }
    }

    return vertices;
  }

  static std::vector<uint32_t> generate_torus_indices(int num_segments_theta, int num_segments_phi)
  {
    std::vector<uint32_t> indices{};

    for (int i = 0; i < num_segments_theta; ++i)
    {
      for (int j = 0; j < num_segments_phi; ++j)
      {
        int first  = i * (num_segments_phi + 1) + j;
        int second = (first + num_segments_phi + 1) % ((num_segments_theta) * (num_segments_phi + 1));
        int third  = (first + 1) % ((num_segments_theta) * (num_segments_phi + 1));
        int fourth = (second + 1) % ((num_segments_theta) * (num_segments_phi + 1));

        indices.push_back(first);
        indices.push_back(second);
        indices.push_back(third);

        indices.push_back(second);
        indices.push_back(fourth);
        indices.push_back(third);
      }
    }

    return indices;
  }

} // namespace mg1

#endif // ESPERT_SANDBOX_TORUSLAYER_HH
