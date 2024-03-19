#ifndef ESPERT_SANDBOX_GRIDCOMPONENT_HH
#define ESPERT_SANDBOX_GRIDCOMPONENT_HH

#include "Espert.hh"
#include "MG1/Interfaces/Interfaces.hh"

using namespace esp;

namespace mg1
{
  class GridComponent : public IComponent
  {
   public:
    GridComponent(uint32_t id);

    static std::tuple<std::vector<Vertex>, std::vector<uint32_t>> construct();
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_GRIDCOMPONENT_HH
