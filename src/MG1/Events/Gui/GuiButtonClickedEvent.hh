#ifndef ESPERT_SANDBOX_GUIBUTTONCLICKEDEVENT_HH
#define ESPERT_SANDBOX_GUIBUTTONCLICKEDEVENT_HH

#include "GuiEvent.hh"

namespace mg1
{
  class GuiButtonClickedEvent : public GuiEvent
  {
   public:
    GuiButtonClickedEvent(const std::string& label) : GuiEvent(label) {}
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIBUTTONCLICKEDEVENT_HH
