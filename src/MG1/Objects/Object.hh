#ifndef ESPERT_SANDBOX_OBJECT_HH
#define ESPERT_SANDBOX_OBJECT_HH

#include "Espert.hh"
#include "MG1/Gui/GuiEvents/GuiEvents.hh"
#include "ObjectState.hh"

namespace mg1
{
  class Object
  {
   private:
    static uint32_t s_id_counter;

   protected:
    static int s_rotation_axis; // 0 = OX, 1 = OY, 2 = OZ

    uint32_t m_id;
    std::string m_name;
    ObjectState m_state;

   public:
    Object(const std::string& name);
    Object();
    ~Object() = default;

    static inline void set_rotation_axis(int axis) { s_rotation_axis = axis; }

    inline uint32_t get_id() { return m_id; }
    inline std::string get_name() { return m_name; }
    inline ObjectState* get_state_handle() { return &m_state; }
    inline bool is_selected() { return m_state.is_selected(); }

    virtual void pre_update(float dt) {}
    virtual void update(float dt) {}
    virtual void post_update(float dt) {}

    virtual void handle_event(GuiInputFloatChangedEvent& event) {}
    virtual void handle_event(GuiInputIntChangedEvent& event) {}
    virtual void handle_event(MouseMovedEvent& event, float dt) {}
    virtual void handle_event(MouseScrolledEvent& event) {}
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECT_HH
