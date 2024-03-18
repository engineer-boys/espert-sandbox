#ifndef ESPERT_SANDBOX_OBJECTADDEDEVENT_HH
#define ESPERT_SANDBOX_OBJECTADDEDEVENT_HH

#include "MG1/Common/ObjectInfo.hh"
#include "MG1/Utils.hh"
#include "ObjectEvent.hh"

using namespace esp;

namespace mg1
{
  class ObjectAddedEvent : public ObjectEvent
  {
   private:
    ObjectInfo* m_info;

   public:
    ObjectAddedEvent(ObjectInfo* info) : ObjectEvent(ObjectLabel::object_created_event), m_info{ info } {}

    inline ObjectInfo* get_info() { return m_info; }

    EVENT_CLASS_TYPE(EventTypeLayer)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTADDEDEVENT_HH
