#ifndef LAYERS_MODEL_EXAMPLE_UTILS_HH
#define LAYERS_MODEL_EXAMPLE_UTILS_HH

// libs
#include "Espert.hh"

using namespace esp;

namespace model_example
{
  struct CameraPush
  {
    glm::mat4 view;
    glm::mat4 proj;
  };

  static std::vector<Vertex> colored_cube{
    // left face
    { { -1, -1, -1 }, { .9f, .9f, .9f } },
    { { -1, 1, -1 }, { .9f, .9f, .9f } },
    { { -1, 1, 1 }, { .9f, .9f, .9f } },
    { { -1, -1, 1 }, { .9f, .9f, .9f } },

    // right face
    { { 1, -1, -1 }, { .8f, .8f, .1f } },
    { { 1, 1, -1 }, { .8f, .8f, .1f } },
    { { 1, 1, 1 }, { .8f, .8f, .1f } },
    { { 1, -1, 1 }, { .8f, .8f, .1f } },

    // bottom face
    { { -1, -1, -1 }, { .9f, .6f, .1f } },
    { { 1, -1, -1 }, { .9f, .6f, .1f } },
    { { 1, -1, 1 }, { .9f, .6f, .1f } },
    { { -1, -1, 1 }, { .9f, .6f, .1f } },

    // top face
    { { -1, 1, -1 }, { .8f, .1f, .1f } },
    { { 1, 1, -1 }, { .8f, .1f, .1f } },
    { { 1, 1, 1 }, { .8f, .1f, .1f } },
    { { -1, 1, 1 }, { .8f, .1f, .1f } },

    // nose face
    { { -1, -1, -1 }, { .1f, .1f, .8f } },
    { { 1, -1, -1 }, { .1f, .1f, .8f } },
    { { 1, 1, -1 }, { .1f, .1f, .8f } },
    { { -1, 1, -1 }, { .1f, .1f, .8f } },

    // tail face
    { { -1, -1, 1 }, { .1f, .8f, .1f } },
    { { 1, -1, 1 }, { .1f, .8f, .1f } },
    { { 1, 1, 1 }, { .1f, .8f, .1f } },
    { { -1, 1, 1 }, { .1f, .8f, .1f } },
  };
  static std::vector<uint32_t> cube_idx{ 0,  2,  1,  2,  0,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
                                         12, 14, 13, 14, 12, 15, 16, 18, 17, 18, 16, 19, 20, 21, 22, 20, 22, 23 };

  static glm::vec3 move_in_circle(glm::vec3 current_position, glm::vec3 circle_center, float dt)
  {
    // Calculate the angle based on time, assuming a constant angular velocity
    float period           = 1.f;
    float angular_velocity = glm::radians(360.f) * period; // 360 degrees per second
    float angle            = angular_velocity * dt;

    // Calculate the position relative to the center of the circle
    glm::vec3 relative_position = current_position - circle_center;

    // Create a rotation matrix around the circle center
    glm::mat4 rotation_mat = glm::rotate(glm::mat4(1.0f), angle, circle_center);

    // Rotate the relative position using the rotation matrix
    glm::vec4 rotated_relative_position = rotation_mat * glm::vec4(relative_position, 1.0f);

    return glm::vec3(rotated_relative_position) + circle_center;
  }
} // namespace model_example

#endif // LAYERS_MODEL_EXAMPLE_UTILS_HH
