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

  enum RotationAxis
  {
    RotationNone,
    RotationOX,
    RotationOY,
    RotationOZ
  };

  struct GuiLabel
  {
    static const std::string rotation_axis;
    static const std::string objects;
    static const std::string object_none;
    static const std::string actions;
    static const std::string action_none;
    static const std::string action_rot_ox;
    static const std::string action_rot_oy;
    static const std::string action_rot_oz;
  };

  inline const std::string GuiLabel::rotation_axis = "Rotation axis";
  inline const std::string GuiLabel::objects       = "Objects";
  inline const std::string GuiLabel::object_none   = "No object";
  inline const std::string GuiLabel::actions       = "Actions";
  inline const std::string GuiLabel::action_none   = "No action";
  inline const std::string GuiLabel::action_rot_ox = "Rotate around OX";
  inline const std::string GuiLabel::action_rot_oy = "Rotate around OY";
  inline const std::string GuiLabel::action_rot_oz = "Rotate around OZ";

} // namespace mg1

#endif // ESPERT_SANDBOX_UTILS_HH
