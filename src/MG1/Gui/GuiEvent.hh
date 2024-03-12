#ifndef ESPERT_SANDBOX_GUIEVENT_HH
#define ESPERT_SANDBOX_GUIEVENT_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  /*template<typename T> class GuiParamChangedEvent : public Event
  {
   private:
    std::string m_label;
    T m_value;

   public:
    GuiParamChangedEvent(const std::string& label, T value) : m_label{ label }, m_value{ value } {}

    //    template<typename U> operator U() = delete;

    inline T get_value() { return m_value; }
    inline bool label_equals(const std::string& second_label) { return m_label == second_label; }

    EVENT_CLASS_TYPE(EventType::EventTypeGui)
    EVENT_CLASS_SUBTYPE(Nothing)
  };*/

  class GuiParamChangedEvent : public Event
  {
   private:
    std::string m_label;

   public:
    GuiParamChangedEvent(const std::string& label) : m_label{ label } {}

    inline bool label_equals(const std::string& second_label) { return m_label == second_label; }

    EVENT_CLASS_TYPE(EventType::EventTypeGui)
    EVENT_CLASS_SUBTYPE(Nothing)
  };

  class GuiFloatParamChangedEvent : public GuiParamChangedEvent
  {
   private:
    float m_value;

   public:
    GuiFloatParamChangedEvent(const std::string& label, float value) : GuiParamChangedEvent(label), m_value{ value } {}

    inline float get_value() { return m_value; }
  };

  class GuiIntParamChangedEvent : public GuiParamChangedEvent
  {
   private:
    int m_value;

   public:
    GuiIntParamChangedEvent(const std::string& label, int value) : GuiParamChangedEvent(label), m_value{ value } {}

    inline int get_value() { return m_value; }
  };

  enum MouseState
  {
    NOT_CAPTURED,
    CAPTURED
  };

  class GuiMouseStateChangedEvent : public Event
  {
   private:
    MouseState m_state;

   public:
    GuiMouseStateChangedEvent(MouseState state) : m_state{ state } {}

    inline MouseState get_state() { return m_state; }

    EVENT_CLASS_TYPE(EventType::EventTypeGui)
    EVENT_CLASS_SUBTYPE(Nothing)
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIEVENT_HH
