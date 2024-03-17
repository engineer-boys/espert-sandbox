#ifndef ESPERT_SANDBOX_GUIEVENT_HH
#define ESPERT_SANDBOX_GUIEVENT_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  class GuiEvent : public Event
  {
   private:
    std::string m_label;

   public:
    GuiEvent(const std::string& label) : m_label{ label } {}

    inline bool label_equals(const std::string& second_label)
    {
      return m_label == second_label;
    } // TODO: overload opeartor

    EVENT_CLASS_TYPE(EventTypeGui)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIEVENT_HH
