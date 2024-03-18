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

   private:
    ObjectState m_state;

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

    virtual void render() = 0;
  };

  struct TorusInfo : public ObjectInfo
  {
    float m_R;
    float m_r;
    int m_density_theta;
    int m_density_phi;

    bool m_dirty{ false };

    TorusInfo(uint32_t id, const std::string& name) : ObjectInfo(id, name) {}

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
    }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTINFO_HH
