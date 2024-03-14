#ifndef ESPERT_SANDBOX_GUIRADIOBUTTON_HH
#define ESPERT_SANDBOX_GUIRADIOBUTTON_HH

#include <memory>

#include "GuiField.hh"
#include "MG1/Gui/GuiEvents/GuiInputIntChangedEvent.hh"

namespace mg1
{
  class GuiRadioButton : public GuiField<int>
  {
   private:
    std::vector<std::string> m_labels;
    int m_prev_value;

   public:
    GuiRadioButton(const std::string& label, int value, std::vector<std::string> labels) :
        GuiField(label, value), m_labels{ labels }, m_prev_value{ value }
    {
    }

    inline void render() override
    {
      for (int i = 0; i < m_labels.size(); i++)
      {
        ImGui::RadioButton(m_labels[i].c_str(), &m_value, i);
        if (i < m_labels.size() - 1) { ImGui::SameLine(); }
      }
    }

    inline std::shared_ptr<Event> create_event() override
    {
      return std::make_shared<GuiInputIntChangedEvent>(m_label, m_value);
    }

    inline virtual bool changed() override
    {
      if (m_prev_value != m_value)
      {
        m_prev_value = m_value;
        return true;
      }
      return false;
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIRADIOBUTTON_HH
