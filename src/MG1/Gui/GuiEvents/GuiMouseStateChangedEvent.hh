#ifndef ESPERT_SANDBOX_GUIMOUSESTATECHANGEDEVENT_HH
#define ESPERT_SANDBOX_GUIMOUSESTATECHANGEDEVENT_HH

#include "GuiEvent.hh"

namespace mg1
{
  class GuiMouseStateChangedEvent : public GuiEvent
  {
   private:
    MouseState m_state;

   public:
    GuiMouseStateChangedEvent(MouseState state) : GuiEvent(""), m_state{ state } {}

    inline MouseState get_state() { return m_state; }

    EVENT_CLASS_TYPE(EventTypeGui)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1
#endif // ESPERT_SANDBOX_GUIMOUSESTATECHANGEDEVENT_HH
