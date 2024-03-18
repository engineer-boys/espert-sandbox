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
    static const std::string create_torus_button;
    static const std::string remove_object_button;
    static const std::string rename_object_button;
  };

  inline const std::string GuiLabel::rotation_axis        = "Rotation axis";
  inline const std::string GuiLabel::objects              = "Objects";
  inline const std::string GuiLabel::object_none          = "No object";
  inline const std::string GuiLabel::actions              = "Actions";
  inline const std::string GuiLabel::action_none          = "No action";
  inline const std::string GuiLabel::action_rot_ox        = "Rotate around OX";
  inline const std::string GuiLabel::action_rot_oy        = "Rotate around OY";
  inline const std::string GuiLabel::action_rot_oz        = "Rotate around OZ";
  inline const std::string GuiLabel::create_torus_button  = "Create torus";
  inline const std::string GuiLabel::remove_object_button = "Remove";
  inline const std::string GuiLabel::rename_object_button = "Rename";

  struct ObjectLabel
  {
    static const std::string object_created_event;
    static const std::string object_removed_event;
  };

  inline const std::string ObjectLabel::object_created_event = "Object created event";
  inline const std::string ObjectLabel::object_removed_event = "Object removed event";

  struct ObjectConstants
  {
    static const glm::vec3 default_color;
    static const glm::vec3 selected_color;
  };

  inline const glm::vec3 ObjectConstants::default_color  = { 1, 1, 1 };
  inline const glm::vec3 ObjectConstants::selected_color = { 1, 1, 0 };
} // namespace mg1

#endif // ESPERT_SANDBOX_UTILS_HH