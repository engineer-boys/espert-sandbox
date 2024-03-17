#ifndef ESPERT_SANDBOX_UTILS_HH
#define ESPERT_SANDBOX_UTILS_HH

#include "Espert.hh"

namespace mg1
{
  enum MouseState
  {
    GuiNotCaptured,
    GuiCaptured
  };

  struct GuiLabel
  {
    static const std::string rotation_axis;
    static const std::string objects;
  };

  inline const std::string GuiLabel::rotation_axis = "Rotation axis";
  inline const std::string GuiLabel::objects       = "Objects";

} // namespace mg1

#endif // ESPERT_SANDBOX_UTILS_HH
