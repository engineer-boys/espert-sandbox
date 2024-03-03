#ifndef ESPERT_SANDBOX_GUILAYER_HH
#define ESPERT_SANDBOX_GUILAYER_HH

#include "Espert.hh"

using namespace esp;

namespace esp_sbx
{
  class GuiLayer : public Layer
  {
   public:
    GuiLayer() = default;

   private:
    inline void update(float dt) override
    {
      if (EspGui::m_use_gui)
      {
        EspGui::new_frame();
        //
        ImGui::Text("FPS: %.1f", 1.f / dt);
        //
        EspGui::end_frame();
      }
    }
  };
} // namespace esp_sbx

#endif // ESPERT_SANDBOX_GUILAYER_HH
