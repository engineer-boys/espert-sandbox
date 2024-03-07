#ifndef ESPERT_SANDBOX_GUIPARAM_HH
#define ESPERT_SANDBOX_GUIPARAM_HH

#include "Espert.hh"

namespace esp_sbx
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
} // namespace esp_sbx

#endif // ESPERT_SANDBOX_GUIPARAM_HH
