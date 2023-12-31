#ifndef LAYERS_PBR_UTILS_HH
#define LAYERS_PBR_UTILS_HH

#include "Espert.hh"

namespace advance_rendering_example
{
  const uint32_t NUMBER_OF_LIGHTS = 4;

  struct QuatVertex_i
  {
    glm::vec2 pos;
    glm::vec2 texCoord;

    QuatVertex_i(glm::vec2 pos, glm::vec2 texCoord) : pos{ pos }, texCoord{ texCoord } {}
  };

  static std::vector<QuatVertex_i> quad_i{ { { -1, -1 }, { 0, 0 } },
                                           { { 1, -1 }, { 1, 0 } },
                                           { { 1, 1 }, { 1, 1 } },
                                           { { -1, 1 }, { 0, 1 } } };

  static std::vector<uint32_t> quad_idx_i{ 0, 1, 2, 2, 3, 0 };

  struct MVP_PBR_Uniform
  {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    alignas(16) glm::vec3 camPos;
  };

  struct Light_PBR_Uniform
  {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;

    Light_PBR_Uniform(glm::vec3 position, glm::vec3 color) : position{ position }, color{ color } {}
  };

  struct MVD_PBR_Uniform
  {
    alignas(16) glm::vec3 moved;
  };

  struct Material_PBR_Uniform
  {
    alignas(16) glm::vec3 moved;
    alignas(16) glm::vec3 albedo;
    alignas(4) float metallic;
    alignas(4) float roughness;
    alignas(4) float ao;
  };

} // namespace advance_rendering_example

#endif // LAYERS_PBR_UTILS_HH