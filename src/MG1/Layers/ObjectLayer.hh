#ifndef ESPERT_SANDBOX_OBJECTLAYER_HH
#define ESPERT_SANDBOX_OBJECTLAYER_HH

#include "Espert.hh"
#include "MG1/Events/Gui/GuiEvents.hh"
#include "MG1/Events/Object/ObjectEvents.hh"
#include "MG1/Utils.hh"

using namespace esp;

namespace mg1
{
  class ObjectLayer : public Layer
  {
   private:
    RotationAxis m_rotation_axis{ RotationNone };
    glm::vec3 m_mouse_cursor_pos{ 0, 0, 0 };

    struct
    {
      std::vector<Node*> m_nodes{};
      glm::vec3 m_mass_centre{ 0, 0, 0 };

      std::vector<Node*>::iterator find(Node* node)
      {
        return std::find_if(m_nodes.begin(), m_nodes.end(), [&](const auto& item) { return item == node; });
      }

      bool try_add(Node* node)
      {
        auto it = find(node);
        if (it == m_nodes.end())
        {
          m_nodes.push_back(node);
          return true;
        }
        return false;
      }
      bool try_remove(Node* node)
      {
        auto it = find(node);
        if (it != m_nodes.end())
        {
          m_nodes.erase(it);
          return true;
        }
        return false;
      }
    } m_selected;

    Scene* m_scene;

    std::shared_ptr<EspShader> m_shader;

   public:
    ObjectLayer(Scene* scene);

    virtual void pre_update(float dt) override;
    virtual void update(float dt) override;
    virtual void post_update(float dt) override;
    virtual void handle_event(Event& event, float dt) override;

   private:
    bool gui_selectable_changed_event_handler(GuiSelectableChangedEvent& event);
    bool gui_button_clicked_event_handler(GuiButtonClickedEvent& event);
    bool cursor_pos_changed_event_handler(CursorPosChangedEvent& event);

    void create_torus(glm::vec3 position = { 0, 0, 0 });
    void create_point(glm::vec3 position = { 0, 0, 0 });
    void remove_object(Node* node, ObjectInfo* info);

    void try_add_node_to_selected(Node* node);
    void try_remove_node_from_selected(Node* node);
    void update_mass_centre();

    // ---------------------------------------- TEMPLATE FUNCTIONS ----------------------------------------
    template<typename ObjComponent> void remove_or_reconstruct()
    {
      for (auto&& [entity, obj, model] : m_scene->get_view<ObjComponent, ModelComponent>())
      {
        if (obj.get_info()->removed()) { remove_object(obj.get_node(), obj.get_info()); }
        else if (obj.get_info()->m_dirty)
        {
          auto [vertices, indices] = obj.reconstruct();
          model.get_model().set_vertex_buffer(vertices);
          model.get_model().set_index_buffer(indices, 0);
        }
      }
    }

    template<typename ObjComponent> void pre_update_unselected_objects()
    {
      for (auto&& [entity, obj] : m_scene->get_view<ObjComponent>())
      {
        if (!obj.get_info()->selected()) { try_remove_node_from_selected(obj.get_node()); }
      }
    }

    template<typename ObjComponent> void pre_update_selected_objects()
    {
      for (auto&& [entity, obj] : m_scene->get_view<ObjComponent>())
      {
        if (obj.get_info()->selected()) { try_add_node_to_selected(obj.get_node()); }
      }
    }

    template<typename ObjComponent> void update_objects()
    {
      auto camera = Scene::get_current_camera();

      for (auto&& [entity, obj, model] : m_scene->get_view<ObjComponent, ModelComponent>())
      {
        auto& uniform_manager = model.get_uniform_manager();
        glm::mat4 mvp         = camera->get_projection() * camera->get_view() * obj.get_node()->get_model_mat();
        uniform_manager.update_buffer_uniform(0, 0, 0, sizeof(glm::mat4), &mvp);

        glm::vec3 color = obj.get_info()->selected() ? ObjectConstants::selected_color : ObjectConstants::default_color;
        uniform_manager.update_buffer_uniform(0, 1, 0, sizeof(glm::vec3), &color);
      }
    }
  };
}; // namespace mg1

#endif // ESPERT_SANDBOX_OBJECTLAYER_HH
