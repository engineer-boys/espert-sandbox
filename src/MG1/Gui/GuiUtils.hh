#ifndef ESPERT_SANDBOX_GUIUTILS_HH
#define ESPERT_SANDBOX_GUIUTILS_HH

#include "Espert.hh"

namespace mg1
{
  enum MouseState
  {
    NotCaptured,
    Captured
  };

  struct GuiFieldLabel
  {
    static const std::string R;
    static const std::string r;
    static const std::string density_theta;
    static const std::string density_phi;
    static const std::string rotation_axis;
    static const std::string objects;
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GUIUTILS_HH
