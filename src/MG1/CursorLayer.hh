#ifndef ESPERT_SANDBOX_CURSORLAYER_HH
#define ESPERT_SANDBOX_CURSORLAYER_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  class CursorLayer : public Layer
  {
   private:
    struct CursorParams
    {
      alignas(16) float m_cursor_size;
      alignas(16) glm::vec3 m_cursor_color;
      float m_aspect_ratio;
    };

    std::shared_ptr<EspShader> m_shader;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

   public:
    CursorLayer();

    void update(float dt) override;
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORLAYER_HH
