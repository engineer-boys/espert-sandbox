#ifndef ESPERT_SANDBOX_CADLAYER_HH
#define ESPERT_SANDBOX_CADLAYER_HH

#include "Espert.hh"
#include "MG1/Events/Gui/GuiEvents.hh"

using namespace esp;

namespace mg1
{
  struct QuadVertex
  {
    glm::vec2 m_pos;
    glm::vec2 m_tex_coord;
  };

  static std::vector<QuadVertex> quad{ { { -1, -1 }, { 0, 1 } },
                                       { { 1, -1 }, { 1, 1 } },
                                       { { 1, 1 }, { 1, 0 } },
                                       { { -1, 1 }, { 0, 0 } } };

  static std::vector<uint32_t> quad_idx{ 0, 1, 2, 2, 3, 0 };

  class CadLayer : public Layer
  {
   private:
    struct
    {
      std::shared_ptr<EspBlock> m_block;
      std::shared_ptr<EspDepthBlock> m_depth_block;
      std::unique_ptr<EspRenderPlan> m_plan;
    } m_scene_render;
    struct
    {
      std::unique_ptr<EspRenderPlan> m_plan;
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
      std::unique_ptr<EspIndexBuffer> m_index_buffer;
    } m_final_render;

    std::shared_ptr<OrbitCamera> m_orbit_camera;
    std::shared_ptr<FpsCamera> m_fps_camera;
    std::shared_ptr<Scene> m_scene;

    std::vector<std::unique_ptr<Layer>> m_children{};

    bool m_none_object_selected{ true };
    bool m_mouse_captured{ true };
    bool m_none_action_selected{ true };

   public:
    CadLayer();

    void pre_update(float dt) override;
    void update(float dt) override;
    void post_update(float dt) override;
    void handle_event(Event& event, float dt) override;

   private:
    bool mouse_moved_event_handler(MouseMovedEvent& event, float dt);
    bool gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event);
    bool gui_mouse_state_changed_event_handler(GuiMouseStateChangedEvent& event);
    bool gui_camera_type_changed_event_handler(GuiCameraTypeChangedEvent& event);

    void handle_keyboard_input(float dt);

    inline bool fps_camera_selected() { return Scene::get_current_camera() == m_fps_camera.get(); }
    inline bool orbit_camera_selected() { return Scene::get_current_camera() == m_orbit_camera.get(); }
  };
} // namespace mg1

#endif // ESPERT_SANDBOX_CADLAYER_HH
