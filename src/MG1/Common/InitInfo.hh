#ifndef ESPERT_SANDBOX_INITINFO_HH
#define ESPERT_SANDBOX_INITINFO_HH

#include "Espert.hh"

namespace mg1
{
  struct CursorInit
  {
    static const esp::ModelParams S_MODEL_PARAMS;
  };

  inline const esp::ModelParams CursorInit::S_MODEL_PARAMS = { .m_position = true, .m_color = true };

  struct GridInit
  {
    static const esp::ModelParams S_MODEL_PARAMS;
  };

  inline const esp::ModelParams GridInit::S_MODEL_PARAMS = { .m_position = true, .m_color = true };

  struct TorusInit
  {
    static constexpr float S_R           = 1.f;
    static constexpr float S_r           = .3f;
    static constexpr int S_DENSITY_THETA = 30; // how many circles
    static constexpr int S_DENSITY_PHI   = 20; // how many points on a circle
    static const esp::ModelParams S_MODEL_PARAMS;
  };

  inline const esp::ModelParams TorusInit::S_MODEL_PARAMS = { .m_position = true };
} // namespace mg1

#endif // ESPERT_SANDBOX_INITINFO_HH
