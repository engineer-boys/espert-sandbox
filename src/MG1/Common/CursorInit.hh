#ifndef ESPERT_SANDBOX_CURSORINIT_HH
#define ESPERT_SANDBOX_CURSORINIT_HH

#include "Espert.hh"

namespace mg1
{
  struct CursorInit
  {
    static const esp::ModelParams S_MODEL_PARAMS;
  };

  inline const esp::ModelParams CursorInit::S_MODEL_PARAMS = { .m_position = true, .m_color = true };

} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORINIT_HH
