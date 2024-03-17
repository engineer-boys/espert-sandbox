#ifndef ESPERT_SANDBOX_GUIFIELD_HH
#define ESPERT_SANDBOX_GUIFIELD_HH

#include "Espert.hh"
#include "MG1/Interfaces/IEventable.hh"

using namespace esp;

namespace mg1
{
  template<typename T> class GuiField : public IEventable
  {
   protected:
    std::string m_label;
    T m_value;

   public:
    GuiField() = default;
    GuiField(const std::string& label, T value) : m_label{ label }, m_value{ value } {}
    virtual ~GuiField() {}

    virtual void render() = 0;
    virtual void create_and_post_event() {}
    virtual inline bool changed() { return ImGui::IsItemDeactivatedAfterEdit(); }
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

} // namespace mg1

#endif // ESPERT_SANDBOX_GUIFIELD_HH
