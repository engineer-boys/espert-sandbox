#include "CursorComponent.hh"
#include "MG1/Events/Object/ObjectEvents.hh"

struct Plane
{
  glm::vec3 n;
  float D;
};

#define SCENE_PLANE \
  Plane { { 0, 1, 0 }, 0 }
#define CURSOR_SIZE     .5f
#define CURSOR_COLOR_OX glm::vec3(1, 0, 0)
#define CURSOR_COLOR_OY glm::vec3(0, 1, 0)
#define CURSOR_COLOR_OZ glm::vec3(0, 0, 1)

static glm::vec3 ray_cast(float x, float y, glm::mat4 view, glm::mat4 projection);
static glm::vec3 intersect_vector_plane(glm::vec3 start, glm::vec3 dir, Plane p);
static std::vector<Vertex> generate_cursor_vertices();
static std::vector<uint32_t> generate_cursor_indices();

namespace mg1
{
  CursorComponent::CursorComponent(uint32_t id, CursorType type, glm::vec3 position) :
      IComponent(id), m_previous_position{ position }
  {
    std::string name;
    switch (type)
    {
    case CursorType::Object:
      name = ObjectLabel::object_cursor;
      break;
    case CursorType::Mouse:
      name = ObjectLabel::mouse_cursor;
      break;
    }

    m_info = std::make_shared<CursorInfo>(m_id, name, type, position);

    ObjectAddedEvent e{ m_info.get() };
    post_event(e);
  }

  std::tuple<std::vector<Vertex>, std::vector<uint32_t>> CursorComponent::construct()
  {
    auto vertices = generate_cursor_vertices();
    auto indices  = generate_cursor_indices();

    return { vertices, indices };
  }

  void CursorComponent::update()
  {
    switch (m_info->m_type)
    {
    case CursorType::Object:
      break;
    case CursorType::Mouse:
      update_mouse();
      break;
    }
  }

  void CursorComponent::update_when_mouse_pressed()
  {
    if (EspInput::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)) { update(); }
  }

  void CursorComponent::update_mouse()
  {
    auto camera = Scene::get_current_camera();
    glm::vec3 ray_mouse =
        ray_cast(EspInput::get_mouse_x_cs(), EspInput::get_mouse_y_cs(), camera->get_view(), camera->get_projection());
    m_previous_position = m_node->get_translation();
    m_node->set_translation(intersect_vector_plane(camera->get_position(), ray_mouse, SCENE_PLANE));
    m_info->m_position = m_node->get_translation();
  }

  void CursorComponent::handle_event(MouseMovedEvent& event, float dt, RotationAxis rotation_axis)
  {
    if (EspInput::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
      switch (rotation_axis)
      {
      case RotationOX:
      {
        m_node->rotate(dt * event.get_dy(), { 1, 0, 0 });
        break;
      }
      case RotationOY:
      {
        m_node->rotate(dt * event.get_dx(), { 0, 1, 0 });
        break;
      }
      case RotationOZ:
      {
        m_node->rotate(dt * (event.get_dx() + event.get_dy()) / 2, { 0, 0, 1 });
        break;
      }
      default:
      {
        break;
      }
      }
    }
  }

  void CursorComponent::handle_event(MouseScrolledEvent& event, ScaleAxis scale_axis)
  {
    float offset_y     = event.get_offset_y();
    float scale_factor = offset_y > 0 ? 1.05f : .95f;

    switch (scale_axis)
    {
    case Scale:
    {
      m_node->scale(scale_factor);
      break;
    }
    case ScaleOX:
    {
      m_node->scale_ox(scale_factor);
      break;
    }
    case ScaleOY:
    {
      m_node->scale_oy(scale_factor);
      break;
    }
    case ScaleOZ:
    {
      m_node->scale_oz(scale_factor);
      break;
    }
    default:
    {
      break;
    }
    }
  }
} // namespace mg1

static glm::vec3 ray_cast(float x, float y, glm::mat4 view, glm::mat4 projection)
{
  glm::vec4 ray_clip  = glm::vec4(x, y, 1.f, 1.f);
  glm::vec4 ray_eye   = glm::inverse(projection) * ray_clip;
  glm::vec4 ray_world = glm::inverse(view) * ray_eye;
  ray_world           = normalize(ray_world);

  return { ray_world.x, ray_world.y, ray_world.z };
}

static glm::vec3 intersect_vector_plane(glm::vec3 start, glm::vec3 dir, Plane p)
{
  float denominator = glm::dot(dir, p.n);

  float t = (p.D - glm::dot(start, p.n)) / denominator;

  return start + t * dir;
}

static void generate_cursor(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {}

static std::vector<Vertex> generate_cursor_vertices()
{
  std::vector<Vertex> vertices{};

  // OX strip
  vertices.push_back({ { 0, 0, 0 }, CURSOR_COLOR_OX });
  vertices.push_back({ { CURSOR_SIZE, 0, 0 }, CURSOR_COLOR_OX });
  // OZ strip
  vertices.push_back({ { 0, 0, 0 }, CURSOR_COLOR_OZ });
  vertices.push_back({ { 0, 0, -CURSOR_SIZE }, CURSOR_COLOR_OZ });
  // OY strip
  vertices.push_back({ { 0, 0, 0 }, CURSOR_COLOR_OY });
  vertices.push_back({ { 0, CURSOR_SIZE, 0 }, CURSOR_COLOR_OY });

  return vertices;
}

static std::vector<uint32_t> generate_cursor_indices()
{
  std::vector<uint32_t> indices = { 0, 1, 0, 2, 3, 2, 4, 5, 4 };

  return indices;
}