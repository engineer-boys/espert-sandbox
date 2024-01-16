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

  static std::vector<Mesh::Vertex> create_cube_vertices()
  {
    std::vector<Mesh::Vertex> vertices{

      // left face (white)
      { { -.5f, -.5f, -.5f }, { .9f, .9f, .9f } },
      { { -.5f, -.5f, .5f }, { .9f, .9f, .9f } },
      { { -.5f, .5f, .5f }, { .9f, .9f, .9f } },
      { { -.5f, -.5f, -.5f }, { .9f, .9f, .9f } },
      { { -.5f, .5f, .5f }, { .9f, .9f, .9f } },
      { { -.5f, .5f, -.5f }, { .9f, .9f, .9f } },

      // right face (yellow)
      { { .5f, -.5f, -.5f }, { .8f, .8f, .1f } },
      { { .5f, .5f, .5f }, { .8f, .8f, .1f } },
      { { .5f, -.5f, .5f }, { .8f, .8f, .1f } },
      { { .5f, -.5f, -.5f }, { .8f, .8f, .1f } },
      { { .5f, .5f, -.5f }, { .8f, .8f, .1f } },
      { { .5f, .5f, .5f }, { .8f, .8f, .1f } },

      // top face (orange, remember y axis points down)
      { { -.5f, -.5f, -.5f }, { .9f, .6f, .1f } },
      { { .5f, -.5f, .5f }, { .9f, .6f, .1f } },
      { { -.5f, -.5f, .5f }, { .9f, .6f, .1f } },
      { { -.5f, -.5f, -.5f }, { .9f, .6f, .1f } },
      { { .5f, -.5f, -.5f }, { .9f, .6f, .1f } },
      { { .5f, -.5f, .5f }, { .9f, .6f, .1f } },

      // bottom face (red)
      { { -.5f, .5f, -.5f }, { .8f, .1f, .1f } },
      { { -.5f, .5f, .5f }, { .8f, .1f, .1f } },
      { { .5f, .5f, .5f }, { .8f, .1f, .1f } },
      { { -.5f, .5f, -.5f }, { .8f, .1f, .1f } },
      { { .5f, .5f, .5f }, { .8f, .1f, .1f } },
      { { .5f, .5f, -.5f }, { .8f, .1f, .1f } },

      // nose face (blue)
      { { -.5f, -.5f, 0.5f }, { .1f, .1f, .8f } },
      { { .5f, .5f, 0.5f }, { .1f, .1f, .8f } },
      { { -.5f, .5f, 0.5f }, { .1f, .1f, .8f } },
      { { -.5f, -.5f, 0.5f }, { .1f, .1f, .8f } },
      { { .5f, -.5f, 0.5f }, { .1f, .1f, .8f } },
      { { .5f, .5f, 0.5f }, { .1f, .1f, .8f } },

      // tail face (green)
      { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
      { { -.5f, .5f, -0.5f }, { .1f, .8f, .1f } },
      { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },
      { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
      { { .5f, .5f, -0.5f }, { .1f, .8f, .1f } },
      { { .5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
    };

    return vertices;
  }

  static std::vector<Mesh::Vertex> create_cube_vertices(glm::vec3 color)
  {
    std::vector<Mesh::Vertex> vertices{

      // left face (white)
      { { -.5f, -.5f, -.5f }, color },
      { { -.5f, -.5f, .5f }, color },
      { { -.5f, .5f, .5f }, color },
      { { -.5f, -.5f, -.5f }, color },
      { { -.5f, .5f, .5f }, color },
      { { -.5f, .5f, -.5f }, color },

      // right face (yellow)
      { { .5f, -.5f, -.5f }, color },
      { { .5f, .5f, .5f }, color },
      { { .5f, -.5f, .5f }, color },
      { { .5f, -.5f, -.5f }, color },
      { { .5f, .5f, -.5f }, color },
      { { .5f, .5f, .5f }, color },

      // top face (orange, remember y axis points down)
      { { -.5f, -.5f, -.5f }, color },
      { { .5f, -.5f, .5f }, color },
      { { -.5f, -.5f, .5f }, color },
      { { -.5f, -.5f, -.5f }, color },
      { { .5f, -.5f, -.5f }, color },
      { { .5f, -.5f, .5f }, color },

      // bottom face (red)
      { { -.5f, .5f, -.5f }, color },
      { { -.5f, .5f, .5f }, color },
      { { .5f, .5f, .5f }, color },
      { { -.5f, .5f, -.5f }, color },
      { { .5f, .5f, .5f }, color },
      { { .5f, .5f, -.5f }, color },

      // nose face (blue)
      { { -.5f, -.5f, 0.5f }, color },
      { { .5f, .5f, 0.5f }, color },
      { { -.5f, .5f, 0.5f }, color },
      { { -.5f, -.5f, 0.5f }, color },
      { { .5f, -.5f, 0.5f }, color },
      { { .5f, .5f, 0.5f }, color },

      // tail face (green)
      { { -.5f, -.5f, -0.5f }, color },
      { { -.5f, .5f, -0.5f }, color },
      { { .5f, .5f, -0.5f }, color },
      { { -.5f, -.5f, -0.5f }, color },
      { { .5f, .5f, -0.5f }, color },
      { { .5f, -.5f, -0.5f }, color },
    };

    return vertices;
  }

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
