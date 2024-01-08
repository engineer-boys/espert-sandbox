#ifndef LAYERS_PBR2_UTILS_HH
#define LAYERS_PBR2_UTILS_HH

#include "Espert.hh"

namespace advance_rendering2_example
{
  struct PosAndTexCoord
  {
    glm::vec2 m_pos;
    glm::vec2 m_tex_coord;
  };

  struct VP_PBR_Uniform2
  {
    glm::mat4 view;
    glm::mat4 proj;
  };

  std::vector<glm::vec3> skybox_vertices2 = {
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

} // namespace advance_rendering2_example

#endif // LAYERS_PBR2_UTILS_HH