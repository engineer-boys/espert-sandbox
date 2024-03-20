#ifndef ESPERT_SANDBOX_COORDINATESYSTEMGRIDLAYER_HH
#define ESPERT_SANDBOX_COORDINATESYSTEMGRIDLAYER_HH

#include "Espert.hh"
#include "MG1/Events/Gui/GuiEvents.hh"

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
    virtual void handle_event(Event& event, float dt) override;

   private:
    bool gui_checkbox_changed_event_handler(GuiCheckboxChangedEvent& event);

    void create_coordinate_system_grid();
    void push_grid();
    void pop_grid();
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_COORDINATESYSTEMGRIDLAYER_HH
