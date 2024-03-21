#ifndef ESPERT_SANDBOX_OBJECTMASSCENTRECHANGEDEVENT_HH
#define ESPERT_SANDBOX_OBJECTMASSCENTRECHANGEDEVENT_HH

#include "MG1/Utils.hh"
#include "ObjectEvent.hh"

using namespace esp;

namespace mg1
{
  class ObjectMassCentreChangedEvent : public ObjectEvent
  {
   private:
    bool m_create;
    glm::vec3 m_position;

   public:
    ObjectMassCentreChangedEvent(bool create, glm::vec3 position = { FLT_MAX, FLT_MAX, FLT_MAX }) :
        ObjectEvent(ObjectLabel::object_mass_centre_changed_event), m_create{ create }, m_position{ position }
    {
    }

    inline bool create() { return m_create; }
    inline glm::vec3 get_position() { return m_position; }

    EVENT_CLASS_TYPE(EventTypeLayer)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTMASSCENTRECHANGEDEVENT_HH
