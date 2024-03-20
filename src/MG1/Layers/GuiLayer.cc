#include "GuiLayer.hh"

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
    ImGui::GetStyle().WindowMinSize = MIN_SIZE;
    ImGui::SetNextWindowPos(INIT_OFFSET, ImGuiCond_Appearing);
    EspGui::begin();

    update_mouse_state();

    ImGui::Text("FPS: %.1f", 1.f / dt);
    ImGui::Spacing();
    glm::vec3 pos = Scene::get_current_camera()->get_position();
    ImGui::Text("Camera pos: (%.2f,%.2f,%.2f)", pos.x, pos.y, pos.z);
    ImGui::Spacing();
    ImGui::Text("Mouse pos: (%.2f,%.2f)", EspInput::get_mouse_x_cs(), EspInput::get_mouse_y_cs());
    ImGui::Spacing();
    ImGui::Text("Cursor pos: (%.2f,%.2f,%.2f)", m_mouse_cursor_pos.x, m_mouse_cursor_pos.y, m_mouse_cursor_pos.z);

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
    Event::try_handler<CursorPosChangedEvent>(event,
                                              ESP_BIND_EVENT_FOR_FUN(GuiLayer::cursor_pos_changed_event_handler));
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

  bool GuiLayer::cursor_pos_changed_event_handler(CursorPosChangedEvent& event)
  {
    if (!(event == ObjectLabel::cursor_pos_changed_event && event.is_type(CursorType::Mouse))) { return false; }
    m_mouse_cursor_pos = event.get_position();

    return true;
  }

  void GuiLayer::update_mouse_state()
  {
    bool window_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    // if (!window_hovered) { ImGui::SetMouseCursor(ImGuiMouseCursor_None); }
    if (m_mouse_state != window_hovered)
    {
      m_mouse_state = (MouseState)window_hovered;
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