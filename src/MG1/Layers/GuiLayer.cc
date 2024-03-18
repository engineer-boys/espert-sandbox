#include "GuiLayer.hh"
#include "MG1/Common/TorusInit.hh"

namespace mg1
{
  GuiLayer::GuiLayer()
  {
    m_actions_combo = std::make_unique<GuiSelectableCombo>(
        GuiLabel::actions,
        GuiSelectables{ std::make_shared<GuiSelectable>(GuiLabel::action_none, true),
                        std::make_shared<GuiSelectable>(GuiLabel::action_rot_ox, false),
                        std::make_shared<GuiSelectable>(GuiLabel::action_rot_oy, false),
                        std::make_shared<GuiSelectable>(GuiLabel::action_rot_oz, false) });
    m_objects_list_box = std::make_unique<GuiObjectInfoSelectableListBox>(
        GuiLabel::objects,
        GuiSelectables{
            std::make_shared<GuiSelectable>(GuiLabel::object_none, true),
        });
    m_create_torus_button = std::make_unique<GuiButton>(GuiLabel::create_torus_button);
    m_create_torus_button->set_max_width();
  }

  void GuiLayer::update(float dt)
  {
    if (!EspGui::m_use_gui) return;

    EspGui::new_frame();
    ImGui::SetNextWindowPos(OFFSET, ImGuiCond_Appearing);
    EspGui::begin();

    update_mouse_state();

    ImGui::Text("FPS: %.1f", 1.f / dt);
    ImGui::Spacing();
    glm::vec3 pos = Scene::get_current_camera()->get_position();
    ImGui::Text("Camera pos: (%.2f,%.2f,%.2f)", pos.x, pos.y, pos.z);
    ImGui::Spacing();
    ImGui::Text("Mouse pos: (%.2f,%.2f)", EspInput::get_mouse_x_cs(), EspInput::get_mouse_y_cs());

    ImGui::SeparatorText("Actions:");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    m_actions_combo->render();

    ImGui::SeparatorText("Objects:");
    m_objects_list_box->render();
    m_create_torus_button->render();

    EspGui::end();

    EspGui::begin();
    ImGui::ShowDemoWindow();
    EspGui::end();

    EspGui::end_frame();
  }

  void GuiLayer::handle_event(esp::Event& event, float dt)
  {
    Event::try_handler<ObjectAddedEvent>(event, ESP_BIND_EVENT_FOR_FUN(GuiLayer::object_added_event_handler));
    Event::try_handler<ObjectRemovedEvent>(event, ESP_BIND_EVENT_FOR_FUN(GuiLayer::object_removed_event_handler));
  }

  bool GuiLayer::object_added_event_handler(ObjectAddedEvent& event)
  {
    if (!(event == ObjectLabel::object_created_event)) { return false; }
    m_objects_list_box->handle_event(event);
    return true;
  }

  bool GuiLayer::object_removed_event_handler(mg1::ObjectRemovedEvent& event)
  {
    if (!(event == ObjectLabel::object_removed_event)) { return false; }
    m_objects_list_box->handle_event(event);
    return true;
  }

  void GuiLayer::update_mouse_state()
  {
    bool mouse_captured = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    // if (!mouse_captured) { ImGui::SetMouseCursor(ImGuiMouseCursor_None); }
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