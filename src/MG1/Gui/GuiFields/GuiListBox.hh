#ifndef ESPERT_SANDBOX_GUILISTBOX_HH
#define ESPERT_SANDBOX_GUILISTBOX_HH

#include <memory>

#include "GuiField.hh"
#include "MG1/Gui/GuiEvents/GuiListBoxChangedEvent.hh"
#include "MG1/Objects/ObjectEvents/ObjectEvents.hh"
#include "MG1/Objects/ObjectUtils.hh"

namespace mg1
{
  class GuiListBox : public GuiField<std::vector<uint32_t>>
  {
   private:
    std::map<uint32_t, ObjectInfo> m_items;
    std::vector<uint32_t> m_prev_selected_items;

   public:
    GuiListBox(const std::string& label, std::map<uint32_t, ObjectInfo> items) :
        GuiField(label, {}), m_items{ items }, m_prev_selected_items{}
    {
    }

    inline void render() override
    {
      if (ImGui::BeginListBox(m_label.c_str(), ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
      {
        for (auto& kv : m_items)
        {
          auto& item         = kv.second;
          auto item_selected = item.m_state->is_selected();

          if (ImGui::Selectable(item.m_name.c_str(), item_selected))
          {
            // Clear selection when CTRL is not held
            if (!ImGui::GetIO().KeyCtrl) { clear(); }
            item.m_state->select();
          }
        }

        update_selected_items();

        ImGui::EndListBox();
      }
    }

    inline std::shared_ptr<Event> create_event() override
    {
      return std::make_shared<GuiListBoxChangedEvent>(m_label, m_value);
    }

    inline virtual bool changed() override
    {
      if (m_prev_selected_items.size() == m_value.size() &&
          std::equal(m_prev_selected_items.begin(), m_prev_selected_items.end(), m_value.begin()))
      {
        return false;
      }
      else
      {
        m_prev_selected_items = m_value;
        return true;
      }
    }

    inline void handle_event(ObjectAddedEvent& event)
    {
      auto item          = event.get_info();
      m_items[item.m_id] = item;
    }

   private:
    inline void update_selected_items()
    {
      if (ImGui::IsItemHovered()) return;

      m_value.clear();
      m_value.reserve(m_items.size());

      for (auto& kv : m_items)
      {
        auto& item = kv.second;

        if (item.m_state->is_selected()) { m_value.push_back(item.m_id); }
      }
    }
    inline void clear()
    {
      for (auto& kv : m_items)
      {
        auto& item = kv.second;
        item.m_state->unselect();
      }
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUILISTBOX_HH
