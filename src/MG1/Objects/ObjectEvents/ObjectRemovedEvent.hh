#ifndef ESPERT_SANDBOX_OBJECTREMOVEDEVENT_HH
#define ESPERT_SANDBOX_OBJECTREMOVEDEVENT_HH

#include "Espert.hh"
#include "MG1/Objects/ObjectUtils.hh"

using namespace esp;

namespace mg1
{
  class ObjectRemovedEvent : public Event
  {
   private:
    ObjectInfo m_info;

   public:
    ObjectRemovedEvent(ObjectInfo info) : m_info{ info } {}

    inline ObjectInfo get_info() { return m_info; }

    EVENT_CLASS_TYPE(EventTypeLayer)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTREMOVEDEVENT_HH
