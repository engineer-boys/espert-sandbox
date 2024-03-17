#include "GuiLayer.hh"
#include "MG1/Common/TorusInit.hh"

namespace mg1
{
  GuiLayer::GuiLayer() { m_objects_list_box = std::make_unique<GuiObjectInfoSelectableListBox>(GuiLabel::objects); }

  void GuiLayer::update(float dt)
  {
    if (!EspGui::m_use_gui) return;

    EspGui::new_frame();
    ImGui::SetNextWindowPos(OFFSET, ImGuiCond_Appearing);
    EspGui::begin();

    update_mouse_state();

    ImGui::Text("FPS: %.1f", 1.f / dt);

    ImGui::SeparatorText("Rotation axis:");
    // update_param(*m_rotation_axis_field);

    ImGui::SeparatorText("Objects:");
    m_objects_list_box->render();

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