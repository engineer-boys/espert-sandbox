#ifndef ESPERT_SANDBOX_TORUSUTILS_HH
#define ESPERT_SANDBOX_TORUSUTILS_HH

namespace mg1
{
  struct TorusInfo
  {
    static constexpr float S_INIT_R           = 1.f;
    static constexpr float S_INIT_r           = .3f;
    static constexpr int S_INIT_DENSITY_THETA = 30; // how many circles
    static constexpr int S_INIT_DENSITY_PHI   = 20; // how many points on a circle
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_TORUSUTILS_HH
