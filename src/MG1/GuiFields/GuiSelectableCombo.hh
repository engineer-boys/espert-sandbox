#ifndef ESPERT_SANDBOX_GUISELECTABLECOMBO_HH
#define ESPERT_SANDBOX_GUISELECTABLECOMBO_HH

#include "GuiField.hh"
#include "GuiSelectable.hh"
#include "MG1/Events/Gui/GuiEvents.hh"

namespace mg1
{
  class GuiSelectableCombo : public GuiField<GuiSelectables>
  {
   private:
    std::string m_preview_value;
    ImGuiComboFlags m_flags;

   public:
    GuiSelectableCombo(const std::string& label, GuiSelectables value = {}, ImGuiComboFlags flags = 0) :
        GuiField(label, value), m_flags{ flags }
    {
      m_preview_value = !m_value.empty() ? m_value[0]->get_label() : "";
    }

    inline void render() override
    {
      if (ImGui::BeginCombo(m_label.c_str(), m_preview_value.c_str(), m_flags))
      {
        for (auto& selectable : m_value)
        {
          bool selected_before_render = selectable->is_selected();

          selectable->render();

          if (selected_before_render && !selectable->is_selected() || selectable->is_selected())
          {
            clear();
            selectable->select();
            m_preview_value = selectable->get_label();
            ImGui::SetItemDefaultFocus();
          }
        }

        ImGui::EndCombo();
      }
    }

   private:
    void clear()
    {
      for (auto& selectable : m_value)
      {
        selectable->unselect();
      }
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUISELECTABLECOMBO_HH
