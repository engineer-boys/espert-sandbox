#ifndef ESPERT_SANDBOX_GUIPARAM_HH
#define ESPERT_SANDBOX_GUIPARAM_HH

#include "Espert.hh"
#include "MG1/Objects/ObjectEvent.hh"
#include "MG1/Objects/ObjectUtils.hh"

namespace mg1
{
  template<typename T> class GuiParam
  {
   protected:
    std::string m_label;
    T m_value;

   public:
    GuiParam(const std::string& label, T value) : m_label{ label }, m_value{ value } {}
    virtual ~GuiParam() {}

    virtual void render()                         = 0;
    virtual std::shared_ptr<Event> create_event() = 0;

    inline const T& get_value() { return m_value; }
    inline const std::string& get_label() { return m_label; }

    inline virtual bool changed() { return ImGui::IsItemDeactivatedAfterEdit(); }
  };

  /*class GuiSliderFloatParam : public GuiParam<float>
  {
   private:
    float m_min_value;
    float m_max_value;

    std::string m_format;

   public:
    GuiSliderFloatParam(const std::string& name,
                        float value,
                        float min_value,
                        float max_value,
                        const std::string& format = "%.2f") :
        GuiParam(name, value), m_min_value{ min_value }, m_max_value{ max_value }, m_format{ format }
    {
    }

    inline void render() override
    {
      ImGui::SliderFloat(m_label.c_str(), &m_value, m_min_value, m_max_value, m_format.c_str());
    }

    inline Event create_event() override
    {

    }
  };*/

  class GuiInputFloatParam : public GuiParam<float>
  {
   private:
    float m_step;
    float m_step_fast;

    float m_min_value{ 0 };
    float m_max_value{ FLT_MAX };

    std::string m_format;

   public:
    GuiInputFloatParam(const std::string& label,
                       float value,
                       float step                = 0.f,
                       float step_fast           = 0.f,
                       const std::string& format = "%.2f") :
        GuiParam(label, value), m_step{ step }, m_step_fast{ step_fast }, m_format{ format }
    {
    }

    inline void render() override
    {
      ImGui::InputFloat(m_label.c_str(), &m_value, m_step, m_step_fast, m_format.c_str());
      m_value = std::clamp(m_value, m_min_value, m_max_value);
    }

    inline std::shared_ptr<Event> create_event() override
    {
      return std::shared_ptr<Event>(new GuiFloatParamChangedEvent(m_label, m_value));
    }
  };

  class GuiInputIntParam : public GuiParam<int>
  {
   private:
    int m_step;
    int m_step_fast;

    int m_min_value{ 1 };
    int m_max_value{ std::numeric_limits<int>::max() };

   public:
    GuiInputIntParam(const std::string& label, int value, int step = 1, int step_fast = 100) :
        GuiParam(label, value), m_step{ step }, m_step_fast{ step_fast }
    {
    }

    inline void render() override
    {
      ImGui::InputInt(m_label.c_str(), &m_value, m_step, m_step_fast);
      m_value = std::clamp(m_value, m_min_value, m_max_value);
    }

    inline std::shared_ptr<Event> create_event() override
    {
      return std::shared_ptr<Event>(new GuiIntParamChangedEvent(m_label, m_value));
    }
  };

  class GuiRadioButtonField : public GuiParam<int>
  {
   private:
    std::vector<std::string> m_labels;
    int m_prev_value;

   public:
    GuiRadioButtonField(const std::string& label, int value, std::vector<std::string> labels) :
        GuiParam(label, value), m_labels{ labels }, m_prev_value{ value }
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
      return std::shared_ptr<Event>(new GuiIntParamChangedEvent(m_label, m_value));
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

  class GuiStringListBoxField : public GuiParam<std::vector<uint32_t>>
  {
   private:
    std::map<uint32_t, ObjectInfo> m_items;
    std::vector<uint32_t> m_prev_selected_items;

   public:
    GuiStringListBoxField(const std::string& label, std::map<uint32_t, ObjectInfo> items) :
        GuiParam(label, {}), m_items{ items }, m_prev_selected_items{}
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
      return std::shared_ptr<Event>(new GuiUintListParamChangedEvent(m_label, m_value)); // TODO: currently redundant
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

#endif // ESPERT_SANDBOX_GUIPARAM_HH
