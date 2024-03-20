#ifndef ESPERT_SANDBOX_COORDINATESYSTEMGRIDLAYER_HH
#define ESPERT_SANDBOX_COORDINATESYSTEMGRIDLAYER_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  class CoordinateSystemGridLayer : public Layer
  {
   private:
    Scene* m_scene;

    std::shared_ptr<EspShader> m_shader;

   public:
    CoordinateSystemGridLayer(Scene* scene);

    virtual void update(float dt) override;

   private:
    void create_coordinate_system_grid();
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_COORDINATESYSTEMGRIDLAYER_HH