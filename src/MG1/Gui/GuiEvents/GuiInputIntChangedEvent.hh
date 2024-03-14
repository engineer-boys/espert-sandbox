#ifndef ESPERT_SANDBOX_GUIINPUTINTCHANGEDEVENT_HH
#define ESPERT_SANDBOX_GUIINPUTINTCHANGEDEVENT_HH

#include "GuiEvent.hh"

namespace mg1
{
  class GuiInputIntChangedEvent : public GuiEvent
  {
   private:
    int m_value;

   public:
    GuiInputIntChangedEvent(const std::string& label, int value) : GuiEvent(label), m_value{ value } {}

    inline int get_value() { return m_value; }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIINPUTINTCHANGEDEVENT_HH
