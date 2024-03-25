#ifndef ESPERT_SANDBOX_GUICAMERATYPECHANGEDEVENT_HH
#define ESPERT_SANDBOX_GUICAMERATYPECHANGEDEVENT_HH

#include "GuiEvent.hh"
#include "MG1/Utils.hh"

namespace mg1
{
  class GuiCameraTypeChangedEvent : public GuiEvent
  {
   private:
    CameraType m_type;

   public:
    GuiCameraTypeChangedEvent(CameraType type) : GuiEvent(GuiLabel::gui_camera_type_changed_event), m_type{ type } {}

    inline CameraType get_type() { return m_type; }
  };
} // namespace mg1
#endif // ESPERT_SANDBOX_GUICAMERATYPECHANGEDEVENT_HH
