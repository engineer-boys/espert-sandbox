#ifndef ESPERT_SANDBOX_GUISELECTABLE_HH
#define ESPERT_SANDBOX_GUISELECTABLE_HH

#include "GuiField.hh"
#include "MG1/Common/ObjectInfo.hh"

namespace mg1
{
  class GuiSelectable : public GuiField<bool>
  {
   protected:
    bool m_prev_selected;

   public:
    GuiSelectable(const std::string& label, bool value) : GuiField<bool>(label, value), m_prev_selected{ value } {}

    inline void render()
    {
      if (ImGui::Selectable(m_label.c_str(), m_value)) { !m_prev_selected ? m_value = true : m_value = false; }

      if (changed())
      {
        m_prev_selected = m_value;
        create_and_post_event();
      }
    }

    inline bool is_selected() { return m_value; }
    inline bool changed() override { return m_value != m_prev_selected; }
  };

  class GuiObjectInfoSelectable : public GuiSelectable
  {
   private:
    ObjectInfo* m_info;

   public:
    GuiObjectInfoSelectable(ObjectInfo* info) : GuiSelectable(info->m_name, info->m_is_selected), m_info{ info } {}

    inline void render() override
    {
      if (ImGui::Selectable(m_label.c_str(), m_value))
      {
        !m_prev_selected ? m_info->m_is_selected = m_value = true : m_info->m_is_selected = m_value = false;
      }

      if (m_value) { m_info->render(); }

      if (changed())
      {
        m_prev_selected = m_value;
        create_and_post_event();
      }
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUISELECTABLE_HH
