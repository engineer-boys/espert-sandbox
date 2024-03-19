#ifndef ESPERT_SANDBOX_CURSORCOMPONENT_HH
#define ESPERT_SANDBOX_CURSORCOMPONENT_HH

#include "Espert.hh"
#include "MG1/Interfaces/Interfaces.hh"

using namespace esp;

namespace mg1
{
  enum class CursorType // TODO: (?)
  {
    Object = 0,
    Mouse  = 1
  };
  
  class CursorComponent : public IComponent
  {
   private:
    glm::vec3 m_position;
    CursorType m_type;

   public:
    CursorComponent(uint32_t id, CursorType type, glm::vec3 position = { 0, 0, 0 });
    ~CursorComponent() = default;

    static std::tuple<std::vector<Vertex>, std::vector<uint32_t>> construct();

    void update();

    inline glm::vec3 get_position() { return m_position; }
    inline bool is_type(CursorType type) { return m_type == type; }

   private:
    void update_mouse();
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CURSORCOMPONENT_HH
