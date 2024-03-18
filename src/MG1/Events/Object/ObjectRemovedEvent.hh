#ifndef ESPERT_SANDBOX_OBJECTREMOVEDEVENT_HH
#define ESPERT_SANDBOX_OBJECTREMOVEDEVENT_HH

#include "MG1/Common/ObjectInfo.hh"
#include "MG1/Utils.hh"
#include "ObjectEvent.hh"

using namespace esp;

namespace mg1
{
  class ObjectRemovedEvent : public ObjectEvent
  {
   private:
    std::string m_name;

   public:
    ObjectRemovedEvent(const std::string& name) : ObjectEvent(ObjectLabel::object_removed_event), m_name{ name } {}

    inline std::string get_name() { return m_name; }

    EVENT_CLASS_TYPE(EventTypeLayer)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTREMOVEDEVENT_HH
