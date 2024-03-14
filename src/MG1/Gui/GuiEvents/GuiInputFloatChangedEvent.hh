#ifndef ESPERT_SANDBOX_GUIINPUTFLOATCHANGEDEVENT_HH
#define ESPERT_SANDBOX_GUIINPUTFLOATCHANGEDEVENT_HH

#include "GuiEvent.hh"

namespace mg1
{
  class GuiInputFloatChangedEvent : public GuiEvent
  {
   private:
    float m_value;

   public:
    GuiInputFloatChangedEvent(const std::string& label, float value) : GuiEvent(label), m_value{ value } {}

    inline float get_value() { return m_value; }
  };

} // namespace mg1

#endif // ESPERT_SANDBOX_GUIINPUTFLOATCHANGEDEVENT_HH
