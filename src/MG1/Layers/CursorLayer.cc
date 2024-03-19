#include "CursorLayer.hh"

#define CURSOR_SIZE     .5f
#define CURSOR_COLOR_OX glm::vec3(1, 0, 0)
#define CURSOR_COLOR_OY glm::vec3(0, 1, 0)
#define CURSOR_COLOR_OZ glm::vec3(0, 0, 1)

glm::vec3 ray_cast(float x, float y, glm::mat4 view, glm::mat4 projection)
{
  glm::vec4 ray_clip  = glm::vec4(x, y, 1.f, 1.f);
  glm::vec4 ray_eye   = glm::inverse(projection) * ray_clip;
  glm::vec4 ray_world = glm::inverse(view) * ray_eye;
  ray_world           = normalize(ray_world);

  return { ray_world.x, ray_world.y, ray_world.z };
}

struct Plane
{
  glm::vec3 n;
  float D;
};

glm::vec3 intersect_vector_plane(glm::vec3 start, glm::vec3 dir, Plane p)
{
  float denominator = glm::dot(dir, p.n);

  float t = (p.D - glm::dot(start, p.n)) / denominator;

  return start + t * dir;
}

struct Pos3ColVertex
{
  glm::vec3 m_pos;
  glm::vec3 m_col;
};

static void generate_cursor(std::vector<Pos3ColVertex>& vertices, std::vector<uint32_t>& indices);

namespace mg1
{
  CursorLayer::CursorLayer()
  {
    // create shader
    {
      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::vec3)); // cursor pos
      uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(glm::mat4)); // vp

      m_shader = ShaderSystem::acquire("Shaders/MG1/CursorLayer/shader");
      m_shader->set_rasterizer_settings(
          { .m_polygon_mode = ESP_POLYGON_MODE_LINE, .m_cull_mode = ESP_CULL_MODE_NONE, .m_line_width = 5.f });
      // m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT, EspCompareOp::ESP_COMPARE_OP_LESS);
      m_shader->set_vertex_layouts(
          { VTX_LAYOUT(sizeof(Pos3ColVertex),
                       0,
                       ESP_VERTEX_INPUT_RATE_VERTEX,
                       ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(Pos3ColVertex, m_pos)),
                       ATTR(1, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, offsetof(Pos3ColVertex, m_col))) });
      m_shader->set_worker_layout(std::move(uniform_meta_data));
      m_shader->build_worker();

      m_uniform_manager = m_shader->create_uniform_manager(0, 0);
      m_uniform_manager->build();
    }

    std::vector<Pos3ColVertex> vertices{};
    std::vector<uint32_t> indices{};
    generate_cursor(vertices, indices);
    m_vertex_buffer = EspVertexBuffer::create(vertices.data(), sizeof(Pos3ColVertex), vertices.size());
    m_index_buffer  = EspIndexBuffer::create(indices.data(), indices.size());
  }

  void CursorLayer::update(float dt)
  {
    if (!m_update) return;

    auto camera = Scene::get_current_camera();

    glm::vec3 ray_mouse =
        ray_cast(EspInput::get_mouse_x_cs(), EspInput::get_mouse_y_cs(), camera->get_view(), camera->get_projection());
    glm::vec3 cursor_pos = intersect_vector_plane(camera->get_position(), ray_mouse, { { 0, 1, 0 }, 0 });
    m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(glm::vec3), &cursor_pos);

    glm::mat4 vp = camera->get_projection() * camera->get_view();
    m_uniform_manager->update_buffer_uniform(0, 1, 0, sizeof(glm::mat4), &vp);

    m_shader->attach();
    m_uniform_manager->attach();
    m_vertex_buffer->attach();
    m_index_buffer->attach();

    EspJob::draw_indexed(m_index_buffer->get_index_count());
  }

  void CursorLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<GuiMouseStateChangedEvent>(
        event,
        ESP_BIND_EVENT_FOR_FUN(CursorLayer::gui_mouse_state_changed_event_handler));
  }

  bool CursorLayer::gui_mouse_state_changed_event_handler(mg1::GuiMouseStateChangedEvent& event)
  {
    m_update = !(bool)event.get_state();
    return false;
  }
} // namespace mg1

static void generate_cursor(std::vector<Pos3ColVertex>& vertices, std::vector<uint32_t>& indices)
{
  // OX strip
  vertices.push_back({ { 0, 0, 0 }, CURSOR_COLOR_OX });
  vertices.push_back({ { CURSOR_SIZE, 0, 0 }, CURSOR_COLOR_OX });
  // OZ strip
  vertices.push_back({ { 0, 0, 0 }, CURSOR_COLOR_OZ });
  vertices.push_back({ { 0, 0, -CURSOR_SIZE }, CURSOR_COLOR_OZ });
  // OY strip
  vertices.push_back({ { 0, 0, 0 }, CURSOR_COLOR_OY });
  vertices.push_back({ { 0, CURSOR_SIZE, 0 }, CURSOR_COLOR_OY });

  indices = { 0, 1, 0, 2, 3, 2, 4, 5, 4 };
}