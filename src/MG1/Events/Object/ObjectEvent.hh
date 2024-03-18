#ifndef ESPERT_SANDBOX_OBJECTEVENT_HH
#define ESPERT_SANDBOX_OBJECTEVENT_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  class ObjectEvent : public Event
  {
   private:
    std::string m_label;

   public:
    ObjectEvent(const std::string& label) : m_label{ label } {}

    bool operator==(const std::string& label) { return m_label == label; }

    EVENT_CLASS_TYPE(EventTypeLayer)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTEVENT_HH
