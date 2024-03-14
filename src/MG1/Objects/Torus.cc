#include "Torus.hh"
#include "MG1/Gui/GuiUtils.hh"

static std::vector<Vertex> generate_torus_vertices(float R, float r, int num_segments_theta, int num_segments_phi);
static std::vector<uint32_t> generate_torus_indices(int num_segments_theta, int num_segments_phi);

namespace mg1
{
  Torus::Torus(float R, float r, int density_theta, int density_phi) : Object()
  {
    m_R             = R;
    m_r             = r;
    m_density_theta = density_theta;
    m_density_phi   = density_phi;

    auto vertices = generate_torus_vertices(m_R, m_r, m_density_theta, m_density_phi);
    auto indices  = generate_torus_indices(m_density_theta, m_density_phi);
    m_node        = Node::create();
    m_model       = std::make_shared<Model>(vertices,
                                      indices,
                                      std::vector<std::shared_ptr<EspTexture>>{},
                                      TorusInit::s_model_params);
  }

  void Torus::pre_update(float dt)
  {
    if (m_needs_reconstruction) { reconstruct_model(); }
  }

  void Torus::update(float dt)
  {
    auto camera = Scene::get_current_camera();

    auto& uniform_manager = m_node->get_entity()->get_component<ModelComponent>().get_uniform_manager();
    glm::mat4 mvp         = camera->get_projection() * /*camera->get_view() **/ m_node->get_model_mat();
    uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &mvp);
  }

  void Torus::handle_event(mg1::GuiInputFloatChangedEvent& event)
  {
    if (event.label_equals(GuiFieldLabel::R)) { m_R = event.get_value(); }
    if (event.label_equals(GuiFieldLabel::r)) { m_r = event.get_value(); }
    m_needs_reconstruction = true;
  }

  void Torus::handle_event(mg1::GuiInputIntChangedEvent& event)
  {
    if (event.label_equals(GuiFieldLabel::density_theta)) { m_density_theta = event.get_value(); }
    if (event.label_equals(GuiFieldLabel::density_phi)) { m_density_phi = event.get_value(); }
    m_needs_reconstruction = true;
  }

  void Torus::handle_event(esp::MouseMovedEvent& event, float dt)
  {
    auto camera = Scene::get_current_camera();

    if (EspInput::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT))
    {
      m_node->translate(glm::vec3(2 * dt * camera->get_delta_move(), 0));
    }

    if (EspInput::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
      switch (Object::s_rotation_axis)
      {
      case 0:
      {
        m_node->rotate_x(2 * dt * camera->get_delta_move().y);
        break;
      }
      case 1:
      {
        m_node->rotate_y(2 * dt * camera->get_delta_move().x);
        break;
      }
      case 2:
      {
        m_node->rotate_z(2 * dt * glm::dot(camera->get_delta_move(), { 1, 1 }) / 2);
        break;
      }
      default:
      {
        break;
      }
      }
    }
  }

  void Torus::handle_event(esp::MouseScrolledEvent& event)
  {
    float offset_y = event.get_offset_y();

    if (offset_y > 0) { m_node->scale(1.1f); }
    else if (offset_y < 0) { m_node->scale(.9f); }
  }

  void Torus::reconstruct_model()
  {
    auto vertices = generate_torus_vertices(m_R, m_r, m_density_theta, m_density_phi);
    auto indices  = generate_torus_indices(m_density_theta, m_density_phi);

    m_model->set_vertex_buffer(vertices);
    m_model->set_index_buffer(indices, 0);

    m_needs_reconstruction = false;
  }

} // namespace mg1

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