#include "CoordinateSystemLayer.hh"

#define GRID_THRESHOLD 30
#define GRID_SCALE     3

struct Pos2ColVertex
{
  glm::vec2 m_pos;
  glm::vec3 m_col;
};

static void generate_grid(std::vector<Pos2ColVertex>& vertices, std::vector<uint32_t>& indices);

namespace mg1
{
  CoordinateSystemLayer::CoordinateSystemLayer()
  {
    // create shader
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4));

      m_shader = ShaderSystem::acquire("Shaders/MG1/CoordinateSystemLayer/shader");
      m_shader->set_rasterizer_settings({ .m_polygon_mode = ESP_POLYGON_MODE_LINE, .m_cull_mode = ESP_CULL_MODE_NONE });
      // m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts(
          { VTX_LAYOUT(sizeof(Pos2ColVertex),
                       0,
                       ESP_VERTEX_INPUT_RATE_VERTEX,
                       ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32_SFLOAT, offsetof(Pos2ColVertex, m_pos)),
                       ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(Pos2ColVertex, m_col))) });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->build_worker();

      m_uniform_manager = m_shader->create_uniform_manager(0, 0);
      m_uniform_manager->build();
    }

    std::vector<Pos2ColVertex> vertices{};
    std::vector<uint32_t> indices{};
    generate_grid(vertices, indices);
    m_vertex_buffer = EspVertexBuffer::create(vertices.data(), sizeof(Pos2ColVertex), vertices.size());
    m_index_buffer  = EspIndexBuffer::create(indices.data(), indices.size());
  }

  void CoordinateSystemLayer::update(float dt)
  {
    auto camera  = Scene::get_current_camera();
    glm::mat4 vp = camera->get_projection() * camera->get_view();

    m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &vp);

    m_shader->attach();
    m_uniform_manager->attach();
    m_vertex_buffer->attach();
    m_index_buffer->attach();

    EspJob::draw_indexed(m_index_buffer->get_index_count());
  }
} // namespace mg1

static void generate_grid(std::vector<Pos2ColVertex>& vertices, std::vector<uint32_t>& indices)
{
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    for (int j = 0; j < GRID_THRESHOLD; j++)
    {
      Pos2ColVertex v{};
      v.m_pos = { i - GRID_THRESHOLD / 2, j - GRID_THRESHOLD / 2 };
      v.m_col = { 1, 1, 1 };

      vertices.push_back(v);
    }
  }

  // columns
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    for (int j = 0; j < GRID_THRESHOLD; j++)
    {
      int idx = i * GRID_THRESHOLD + j;

      if (j < GRID_THRESHOLD - 2)
      {
        indices.push_back(idx);
        indices.push_back(idx + 1);
        indices.push_back(idx + 2);
      }
    }
  }

  // rows
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    for (int j = 0; j < GRID_THRESHOLD; j++)
    {
      int idx = i * GRID_THRESHOLD + j;

      if (i < GRID_THRESHOLD - 2)
      {
        indices.push_back(idx);
        indices.push_back(idx + GRID_THRESHOLD);
        indices.push_back(idx + GRID_THRESHOLD * 2);
      }
    }
  }

  // OX
  int offset = vertices.size();
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    Pos2ColVertex v{};
    v.m_pos = { i - GRID_THRESHOLD / 2, 0 };
    v.m_col = { 1, 0, 0 };

    int idx = i + offset;

    vertices.push_back(v);

    if (i < GRID_THRESHOLD - 2)
    {
      indices.push_back(idx);
      indices.push_back(idx + 1);
      indices.push_back(idx + 2);
    }
  }

  // OY
  offset = vertices.size();
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    Pos2ColVertex v{};
    v.m_pos = { 0, i - GRID_THRESHOLD / 2 };
    v.m_col = { 0, 0, 1 };

    int idx = i + offset;

    vertices.push_back(v);

    if (i < GRID_THRESHOLD - 2)
    {
      indices.push_back(idx);
      indices.push_back(idx + 1);
      indices.push_back(idx + 2);
    }
  }

  for (auto& v : vertices)
  {
    v.m_pos *= GRID_SCALE;
  }
}