#include "PointComponent.hh"
#include "MG1/Events/Object/ObjectEvents.hh"

static void generate_point(float r, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

namespace mg1
{
  PointComponent::PointComponent(uint32_t id, float r) : IComponent(id)
  {
    m_info      = std::make_shared<PointInfo>(m_id, "Point " + std::to_string(m_id));
    m_info->m_r = r;

    ObjectAddedEvent e{ m_info.get() };
    post_event(e);
  }

  std::tuple<std::vector<Vertex>, std::vector<uint32_t>> PointComponent::reconstruct()
  {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
    generate_point(m_info->m_r, vertices, indices);

    return { vertices, indices };
  }

  void PointComponent::handle_event(CursorPosChangedEvent& event)
  {
    if (EspInput::is_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT))
    {
      auto d_pos = event.get_delta_position();
      m_node->translate({ d_pos.x, 0, 0 });
      if (EspInput::is_key_pressed(GLFW_KEY_Y)) { m_node->translate({ 0, -d_pos.z, 0 }); }
      if (EspInput::is_key_pressed(GLFW_KEY_Z)) { m_node->translate({ 0, 0, d_pos.z }); }
    }
  }
} // namespace mg1

static void generate_point(float r, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
  const int stacks = 20; // Number of stacks (horizontal slices)
  const int slices = 20; // Number of slices (vertical segments)

  for (int i = 0; i <= stacks; ++i)
  {
    float phi = glm::pi<float>() * i / stacks;
    // float phiNext = glm::pi<float>() * (i + 1) / stacks;

    for (int j = 0; j <= slices; ++j)
    {
      float theta = 2 * glm::pi<float>() * j / slices;

      Vertex vertex;
      vertex.m_position.x = r * sin(phi) * cos(theta);
      vertex.m_position.y = r * cos(phi);
      vertex.m_position.z = r * sin(phi) * sin(theta);

      vertices.push_back(vertex);

      if (i < stacks && j < slices)
      {
        unsigned int idx      = i * (slices + 1) + j;
        unsigned int next_idx = idx + slices + 1;

        indices.push_back(idx);
        indices.push_back(next_idx);
        indices.push_back(idx + 1);

        indices.push_back(next_idx);
        indices.push_back(next_idx + 1);
        indices.push_back(idx + 1);
      }
    }
  }
}