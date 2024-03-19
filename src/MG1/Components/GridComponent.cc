#include "GridComponent.hh"

#define GRID_THRESHOLD 30
#define GRID_SCALE     3

static void generate_grid(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

namespace mg1
{

  GridComponent::GridComponent(uint32_t id) : IComponent(id) {}

  std::tuple<std::vector<Vertex>, std::vector<uint32_t>> GridComponent::construct()
  {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
    generate_grid(vertices, indices);

    return { vertices, indices };
  }
} // namespace mg1

static void generate_grid(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
  // OX
  int offset = vertices.size();
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    Vertex v{};
    v.m_position = { i - GRID_THRESHOLD / 2, 0, 0 };
    v.m_color    = { 1, 0, 0 };

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
    Vertex v{};
    v.m_position = { 0, 0, i - GRID_THRESHOLD / 2 };
    v.m_color    = { 0, 0, 1 };

    int idx = i + offset;

    vertices.push_back(v);

    if (i < GRID_THRESHOLD - 2)
    {
      indices.push_back(idx);
      indices.push_back(idx + 1);
      indices.push_back(idx + 2);
    }
  }

  offset = vertices.size();
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    for (int j = 0; j < GRID_THRESHOLD; j++)
    {
      Vertex v{};
      v.m_position = { i - GRID_THRESHOLD / 2, 0, j - GRID_THRESHOLD / 2 };
      v.m_color    = { 1, 1, 1 };

      vertices.push_back(v);
    }
  }

  // columns
  for (int i = 0; i < GRID_THRESHOLD; i++)
  {
    for (int j = 0; j < GRID_THRESHOLD; j++)
    {
      int idx = i * GRID_THRESHOLD + j + offset;

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
      int idx = i * GRID_THRESHOLD + j + offset;

      if (i < GRID_THRESHOLD - 2)
      {
        indices.push_back(idx);
        indices.push_back(idx + GRID_THRESHOLD);
        indices.push_back(idx + GRID_THRESHOLD * 2);
      }
    }
  }

  for (auto& v : vertices)
  {
    v.m_position *= GRID_SCALE;
  }
}
