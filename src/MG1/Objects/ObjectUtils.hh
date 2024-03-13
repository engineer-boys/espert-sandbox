#ifndef ESPERT_SANDBOX_OBJECTUTILS_HH
#define ESPERT_SANDBOX_OBJECTUTILS_HH

#include "Espert.hh"
#include "ObjectState.hh"

using namespace esp;

namespace mg1
{
  struct TorusInfo
  {
    static constexpr float S_INIT_R           = 1.f;
    static constexpr float S_INIT_r           = .3f;
    static constexpr int S_INIT_DENSITY_THETA = 30; // how many circles
    static constexpr int S_INIT_DENSITY_PHI   = 20; // how many points on a circle

    static const ModelParams s_model_params;
  };

  struct ObjectInfo
  {
    uint32_t m_id;
    std::string m_name;
    ObjectState* m_state;
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTUTILS_HH
