#ifndef ESPERT_SANDBOX_TORUS_HH
#define ESPERT_SANDBOX_TORUS_HH

#include "Espert.hh"
#include "Object.hh"
#include "ObjectUtils.hh"

using namespace esp;

namespace mg1
{
  class Torus : public Object
  {
   private:
    float m_R;
    float m_r;
    int m_density_theta;
    int m_density_phi;

    std::shared_ptr<Model> m_model;
    std::shared_ptr<Node> m_node;

    bool m_needs_reconstruction{ false };

   public:
    Torus(float R           = TorusInfo::S_INIT_R,
          float r           = TorusInfo::S_INIT_r,
          int density_theta = TorusInfo::S_INIT_DENSITY_THETA,
          int density_phi   = TorusInfo::S_INIT_DENSITY_PHI);
    ~Torus() = default;

    void pre_update(float dt) override;
    void update(float dt) override;

    void handle_event(GuiFloatParamChangedEvent& event) override;
    void handle_event(GuiIntParamChangedEvent& event) override;
    void handle_event(MouseMovedEvent& event, float dt) override;
    void handle_event(MouseScrolledEvent& event) override;

    inline std::shared_ptr<Model>& get_model() { return m_model; }
    inline std::shared_ptr<Node>& get_node() { return m_node; }

   private:
    void reconstruct_model();
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_TORUS_HH
