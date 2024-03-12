#ifndef ESPERT_SANDBOX_GUILAYER_HH
#define ESPERT_SANDBOX_GUILAYER_HH

#include "Espert.hh"
#include "GuiEvent.hh"
#include "GuiParam.hh"
#include "MG1/TorusUtils.hh"

using namespace esp;

namespace mg1
{
  template<typename T> using Params = std::vector<std::unique_ptr<GuiParam<T>>>;

  class GuiLayer : public Layer
  {
   private:
    static constexpr ImVec2 OFFSET{ 10, 10 };

    Params<float> m_float_params{};
    Params<int> m_int_params{};

    GuiRadioButtonField m_rotation_axis_field{ "Rotation axis", 0, { "X", "Y", "Z" } };

    MouseState m_mouse_state{ NOT_CAPTURED };

   public:
    GuiLayer()
    {
      // create gui params
      m_float_params.emplace_back(new GuiInputFloatParam("R", mg1::TorusInfo::S_INIT_R, 0.05f, 1.0f));
      m_float_params.emplace_back(new GuiInputFloatParam("r", mg1::TorusInfo::S_INIT_r, 0.05f, 1.0f));
      m_int_params.emplace_back(new GuiInputIntParam("Density - theta", mg1::TorusInfo::S_INIT_DENSITY_THETA));
      m_int_params.emplace_back(new GuiInputIntParam("Density - phi", mg1::TorusInfo::S_INIT_DENSITY_PHI));
    }

   private:
    inline void update(float dt) override
    {
      if (EspGui::m_use_gui)
      {
        EspGui::new_frame();
        ImGui::SetNextWindowPos(OFFSET, ImGuiCond_Appearing);
        EspGui::begin();

        ImGui::Text("FPS: %.1f", 1.f / dt);
        ImGui::Spacing();
        render_params(m_float_params);
        ImGui::Spacing();
        render_params(m_int_params);
        ImGui::SeparatorText("Rotation axis");
        m_rotation_axis_field.render();
        if (m_rotation_axis_field.changed()) { post_event(*m_rotation_axis_field.create_event()); }
        ImGui::Spacing();

        bool mouse_captured = ImGui::IsWindowHovered();
        if (!mouse_captured) { ImGui::SetMouseCursor(ImGuiMouseCursor_None); }
        if (m_mouse_state != mouse_captured)
        {
          m_mouse_state = (MouseState)mouse_captured;
          GuiMouseStateChangedEvent mouse_state_changed_event{ m_mouse_state };
          post_event(mouse_state_changed_event);
        }

        EspGui::end();
        EspGui::end_frame();
      }
    }

    template<typename T> inline void render_params(const Params<T>& params)
    {
      for (auto& param : params)
      {
        param->render();
        if (param->changed()) { post_event(*param->create_event()); }
      }
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUILAYER_HH
