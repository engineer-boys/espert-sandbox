#ifndef ESPERT_SANDBOX_GUISELECTABLECHANGEDEVENT_HH
#define ESPERT_SANDBOX_GUISELECTABLECHANGEDEVENT_HH

#include "GuiEvent.hh"

namespace mg1
{
  class GuiSelectableChangedEvent : public GuiEvent
  {
   private:
    bool m_value;

   public:
    GuiSelectableChangedEvent(const std::string& label, bool value) : GuiEvent(label), m_value{ value } {}

    inline bool get_value() { return m_value; }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUISELECTABLECHANGEDEVENT_HH
