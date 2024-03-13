#ifndef ESPERT_SANDBOX_OBJECTEVENT_HH
#define ESPERT_SANDBOX_OBJECTEVENT_HH

#include "Espert.hh"
#include "ObjectUtils.hh"

using namespace esp;

namespace mg1
{
  class ObjectAddedEvent : public Event
  {
   private:
    ObjectInfo m_info;

   public:
    ObjectAddedEvent(ObjectInfo info) : m_info{ info } {}

    inline ObjectInfo get_info() { return m_info; }

    EVENT_CLASS_TYPE(EventTypeLayer)
    EVENT_CLASS_SUBTYPE(Nothing)
  };

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

#endif // ESPERT_SANDBOX_OBJECTEVENT_HH
