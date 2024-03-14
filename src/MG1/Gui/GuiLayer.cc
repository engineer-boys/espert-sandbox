#include "GuiLayer.hh"
#include "GuiUtils.hh"

namespace mg1
{
  GuiLayer::GuiLayer()
  {
    m_torus_params.m_R = std::make_unique<GuiInputFloat>(GuiFieldLabel::R, TorusInit::S_R, 0.05f, 1.0f);
    m_torus_params.m_r = std::make_unique<GuiInputFloat>(GuiFieldLabel::r, TorusInit::S_r, 0.05f, 1.0f);
    m_torus_params.m_density_theta =
        std::make_unique<GuiInputInt>(GuiFieldLabel::density_theta, TorusInit::S_DENSITY_THETA);
    m_torus_params.m_density_phi = std::make_unique<GuiInputInt>(GuiFieldLabel::density_phi, TorusInit::S_DENSITY_PHI);

    m_rotation_axis_field =
        std::make_unique<GuiRadioButton>(GuiFieldLabel::rotation_axis, 0, std::vector<std::string>{ "X", "Y", "Z" });

    m_objects_list_box = std::make_unique<GuiListBox>(GuiFieldLabel::objects, std::map<uint32_t, ObjectInfo>{});
  }

  void GuiLayer::update(float dt)
  {
    if (!EspGui::m_use_gui) return;

    EspGui::new_frame();
    ImGui::SetNextWindowPos(OFFSET, ImGuiCond_Appearing);
    EspGui::begin();

    update_mouse_state();

    ImGui::Text("FPS: %.1f", 1.f / dt);

    ImGui::SeparatorText("Torus parameters:");
    update_param(*m_torus_params.m_R);
    update_param(*m_torus_params.m_r);
    ImGui::Spacing();
    update_param(*m_torus_params.m_density_theta);
    update_param(*m_torus_params.m_density_phi);

    ImGui::SeparatorText("Rotation axis:");
    update_param(*m_rotation_axis_field);

    ImGui::SeparatorText("Objects:");
    help_marker("Hold CTRL and click to select multiple items.");
    update_param(*m_objects_list_box);

    EspGui::end();

    EspGui::begin();
    ImGui::ShowDemoWindow();
    EspGui::end();

    EspGui::end_frame();
  }

  void GuiLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<ObjectAddedEvent>(event, ESP_BIND_EVENT_FOR_FUN(GuiLayer::object_added_event_handler));
    // Event::try_handler<ObjectRemovedEvent>(event, ESP_BIND_EVENT_FOR_FUN(GuiLayer::object_removed_event_handler));
  }

  bool GuiLayer::object_added_event_handler(ObjectAddedEvent& event)
  {
    m_objects_list_box->handle_event(event);
    return true;
  }

  void GuiLayer::update_mouse_state()
  {
    bool mouse_captured = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    if (!mouse_captured) { ImGui::SetMouseCursor(ImGuiMouseCursor_None); }
    if (m_mouse_state != mouse_captured)
    {
      m_mouse_state = (MouseState)mouse_captured;
      GuiMouseStateChangedEvent mouse_state_changed_event{ m_mouse_state };
      post_event(mouse_state_changed_event);
    }
  }

  void GuiLayer::help_marker(const char* desc)
  {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }
} // namespace mg1