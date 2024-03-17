#ifndef ESPERT_SANDBOX_GUISELECTABLELISTBOXANYSELECTEDEVENT_HH
#define ESPERT_SANDBOX_GUISELECTABLELISTBOXANYSELECTEDEVENT_HH

#include "GuiEvent.hh"

namespace mg1
{
  class GuiSelectableListBoxAnySelectedEvent : public GuiEvent
  {
   private:
    bool m_value;

   public:
    GuiSelectableListBoxAnySelectedEvent(const std::string& label, bool value) : GuiEvent(label), m_value{ value } {}

    inline bool get_value() { return m_value; }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUISELECTABLELISTBOXANYSELECTEDEVENT_HH
