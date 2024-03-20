#ifndef ESPERT_SANDBOX_CURSORPOSCHANGEDEVENT_HH
#define ESPERT_SANDBOX_CURSORPOSCHANGEDEVENT_HH

#include "MG1/Components/CursorComponent.hh"
#include "MG1/Utils.hh"
#include "ObjectEvent.hh"

using namespace esp;

namespace mg1
{
  class CursorPosChangedEvent : public ObjectEvent
  {
   private:
    CursorType m_type;
    glm::vec3 m_position;
    glm::vec3 m_delta_position;

   public:
    CursorPosChangedEvent(CursorType type, glm::vec3 position, glm::vec3 delta) :
        ObjectEvent(ObjectLabel::cursor_pos_changed_event), m_type{ type }, m_position{ position },
        m_delta_position{ delta }
    {
    }

    inline bool is_type(CursorType type) { return m_type == type; }
    inline glm::vec3 get_position() { return m_position; }
    inline glm::vec3 get_delta_position() { return m_delta_position; }

    EVENT_CLASS_TYPE(EventTypeLayer)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORPOSCHANGEDEVENT_HH
