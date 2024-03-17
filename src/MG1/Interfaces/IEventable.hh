#ifndef ESPERT_SANDBOX_IEVENTABLE_HH
#define ESPERT_SANDBOX_IEVENTABLE_HH

#include "Espert.hh"

using namespace esp;

namespace mg1
{
  class IEventable
  {
   private:
    EventManagerFun m_events_manager_fun;

   public:
    IEventable() : m_events_manager_fun{ EspApplication::get_event_manager() } {}

    inline void post_event(Event& event) { m_events_manager_fun(event); }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_IEVENTABLE_HH
