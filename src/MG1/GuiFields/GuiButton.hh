#ifndef ESPERT_SANDBOX_GUIBUTTON_HH
#define ESPERT_SANDBOX_GUIBUTTON_HH

#include "GuiField.hh"

namespace mg1
{
  class GuiButton;
  using GuiButtons = std::vector<std::shared_ptr<GuiButton>>;

  class GuiButton : public GuiField<void*> // TODO: std::function<void()>
  {
   private:
    glm::vec2 m_size;
    bool m_max_width{ false };

    bool m_clicked;

   public:
    GuiButton(const std::string& label, glm::vec2 size = { 0, 0 }) : GuiField(label, nullptr), m_size{ size } {}

    inline void set_size(glm::vec2 v) { m_size = v; }
    inline void set_size(ImVec2 v) { m_size = { v.x, v.y }; }
    inline void set_max_width() { m_max_width = true; }

    inline void render() override
    {
      if (m_max_width) { m_size.x = ImGui::GetContentRegionAvail().x; }
      if ((m_clicked = ImGui::Button(m_label.c_str(), ImVec2(m_size.x, m_size.y)))) {}

      if (clicked()) { create_and_post_event(); }
    }

    inline bool clicked() { return m_clicked; }

    inline void create_and_post_event() override {}
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIBUTTON_HH
