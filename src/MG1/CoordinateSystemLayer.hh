#ifndef ESPERT_SANDBOX_COORDINATESYSTEMLAYER_HH
#define ESPERT_SANDBOX_COORDINATESYSTEMLAYER_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  class CoordinateSystemLayer : public Layer
  {
   private:
    std::shared_ptr<EspShader> m_shader;
    std::unique_ptr<EspUniformManager> m_uniform_manager;

    std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    std::unique_ptr<EspIndexBuffer> m_index_buffer;

   public:
    CoordinateSystemLayer();

    virtual void update(float dt) override;
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_COORDINATESYSTEMLAYER_HH
