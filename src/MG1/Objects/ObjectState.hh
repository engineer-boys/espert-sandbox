#ifndef ESPERT_SANDBOX_OBJECTSTATE_HH
#define ESPERT_SANDBOX_OBJECTSTATE_HH

namespace mg1
{
  class ObjectState
  {
   private:
    bool m_is_selected;

   public:
    ObjectState(bool is_selected) : m_is_selected{ is_selected } {}

    inline bool is_selected() { return m_is_selected; }
    inline void select() { m_is_selected = true; }
    inline void unselect() { m_is_selected = false; }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTSTATE_HH
