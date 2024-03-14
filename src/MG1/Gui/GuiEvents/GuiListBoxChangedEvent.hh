#ifndef ESPERT_SANDBOX_GUILISTBOXCHANGEDEVENT_HH
#define ESPERT_SANDBOX_GUILISTBOXCHANGEDEVENT_HH

#include "GuiEvent.hh"

namespace mg1
{
  class GuiListBoxChangedEvent : public GuiEvent
  {
   private:
    std::vector<uint32_t> m_value;

   public:
    GuiListBoxChangedEvent(const std::string& label, std::vector<uint32_t> value) : GuiEvent(label), m_value{ value } {}

    inline std::vector<uint32_t> get_value() { return m_value; }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUILISTBOXCHANGEDEVENT_HH
