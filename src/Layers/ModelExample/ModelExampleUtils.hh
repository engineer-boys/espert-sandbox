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

  static std::vector<Model::Vertex> create_cube_vertices()
  {
    std::vector<Model::Vertex> vertices{

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
} // namespace model_example

#endif // LAYERS_MODEL_EXAMPLE_UTILS_HH
