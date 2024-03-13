#include "Object.hh"

namespace mg1
{
  uint32_t Object::s_id_counter = 0;

  int Object::s_rotation_axis = 0;

  Object::Object(const std::string& name) : Object() { m_name = name; }

  Object::Object() : m_id{ s_id_counter++ }, m_name{ std::to_string(m_id) }, m_state{ false } {}
} // namespace mg1