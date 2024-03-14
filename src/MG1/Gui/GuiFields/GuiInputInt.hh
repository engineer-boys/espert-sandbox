#ifndef ESPERT_SANDBOX_GUIINPUTINT_HH
#define ESPERT_SANDBOX_GUIINPUTINT_HH

#include <memory>

#include "GuiField.hh"
#include "MG1/Gui/GuiEvents/GuiInputIntChangedEvent.hh"

namespace mg1
{
  class GuiInputInt : public GuiField<int>
  {
   private:
    int m_step;
    int m_step_fast;

    int m_min_value{ 1 };
    int m_max_value{ std::numeric_limits<int>::max() };

   public:
    GuiInputInt(const std::string& label, int value, int step = 1, int step_fast = 100) :
        GuiField(label, value), m_step{ step }, m_step_fast{ step_fast }
    {
    }

    inline void render() override
    {
      ImGui::InputInt(m_label.c_str(), &m_value, m_step, m_step_fast);
      m_value = std::clamp(m_value, m_min_value, m_max_value);
    }

    inline std::shared_ptr<Event> create_event() override
    {
      return std::make_shared<GuiInputIntChangedEvent>(m_label, m_value);
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIINPUTINT_HH
