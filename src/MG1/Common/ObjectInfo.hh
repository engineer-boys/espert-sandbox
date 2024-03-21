#ifndef ESPERT_SANDBOX_OBJECTINFO_HH
#define ESPERT_SANDBOX_OBJECTINFO_HH

#include "Espert.hh"

namespace mg1
{
  struct ObjectInfo
  {
   public:
    enum class ObjectState
    {
      None     = 0,
      Selected = 1,
      Removed  = 2
    };

   protected:
    ObjectState m_state;
    bool m_renameable{ false };
    bool m_removeable{ false };

   public:
    uint32_t m_id;
    std::string m_name;

    ObjectInfo(uint32_t id, const std::string& name) : m_id{ id }, m_name{ name }, m_state{ ObjectState::None } {}

    inline void select()
    {
      if (m_state == ObjectState::None) { m_state = ObjectState::Selected; }
    }
    inline void unselect()
    {
      if (m_state == ObjectState::Selected) { m_state = ObjectState::None; }
    }
    inline void remove() { m_state = ObjectState::Removed; }

    inline bool selected() { return m_state == ObjectState::Selected; }
    inline bool removed() { return m_state == ObjectState::Removed; }
    inline bool is_renameable() { return m_renameable; }
    inline bool is_removeable() { return m_removeable; }

    virtual void render() = 0;
  };

  struct TorusInfo : public ObjectInfo
  {
    float m_R;
    float m_r;
    int m_density_theta;
    int m_density_phi;

    bool m_dirty{ false };

    TorusInfo(uint32_t id, const std::string& name) : ObjectInfo(id, name) { m_renameable = m_removeable = true; }

    inline void render() override
    {
      ImGui::SeparatorText("Info:");
      ImGui::Spacing();
      ImGui::InputText("Name", &m_name);
      ImGui::InputFloat("R", &m_R, 0.05f, 1.0f, "%.2f");
      if (ImGui::IsItemDeactivatedAfterEdit()) { m_dirty = true; }
      ImGui::InputFloat("r", &m_r, 0.05f, 1.0f, "%.2f");
      if (ImGui::IsItemDeactivatedAfterEdit()) { m_dirty = true; }
      ImGui::InputInt("Density - theta", &m_density_theta, 1, 100);
      if (ImGui::IsItemDeactivatedAfterEdit()) { m_dirty = true; }
      ImGui::InputInt("Density - phi", &m_density_phi, 1, 100);
      if (ImGui::IsItemDeactivatedAfterEdit()) { m_dirty = true; }
      ImGui::Spacing();
    };
  };

  struct PointInfo : public ObjectInfo
  {
    float m_r;

    bool m_dirty{ false };

    PointInfo(uint32_t id, const std::string& name) : ObjectInfo(id, name) { m_renameable = m_removeable = true; }

    inline void render() override
    {
      ImGui::SeparatorText("Info:");
      ImGui::Spacing();
      ImGui::InputText("Name", &m_name);
      ImGui::InputFloat("R", &m_r, 0.05f, 1.0f, "%.2f");
      if (ImGui::IsItemDeactivatedAfterEdit()) { m_dirty = true; }
      ImGui::Spacing();
    }
  };

  enum class CursorType
  {
    Object = 0,
    Mouse  = 1
  };

  struct CursorInfo : public ObjectInfo
  {
    CursorType m_type;
    glm::vec3 m_position;

   public:
    CursorInfo(uint32_t id, const std::string& name, CursorType type, glm::vec3 position) :
        ObjectInfo(id, name), m_type{ type }, m_position{ position }
    {
    }

    inline void render() override
    {
      ImGui::SeparatorText("Info:");
      ImGui::Text("Name %s", m_name.c_str());
      ImGui::Spacing();
      if (m_type == CursorType::Mouse)
      {
        ImGui::InputFloat("x", &m_position.x, 0.05f, 1.0f, "%.2f");
        ImGui::InputFloat("z", &m_position.z, 0.05f, 1.0f, "%.2f");
      }
      ImGui::Spacing();
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTINFO_HH
