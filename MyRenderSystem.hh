#ifndef ESPERT_SANDBOX__MY_RENDER_SYSTEM_H_
#define ESPERT_SANDBOX__MY_RENDER_SYSTEM_H_

#include "MyGameObject.hh"

// libs
#include "Espert.hh"

namespace my_game
{
  class MyRenderSystem
  {
   private:
    struct MyUniformData
    {
      glm::mat4 camera_projection{ 1.f };
    };

    struct MyPushConstantData : esp::EspPushConstantData
    {
      glm::mat4 transform{ 1.f };
    };

    esp::EspDevice& m_device;

    std::vector<std::unique_ptr<esp::EspBuffer>> m_uniform_buffers{};
    std::unique_ptr<esp::EspDescriptorSetLayout> m_descriptor_set_layout{};
    std::vector<std::unique_ptr<esp::EspDescriptorSet>> m_descriptor_sets{};
    std::unique_ptr<esp::EspDescriptorPool> m_descriptor_pool{};

    std::unique_ptr<esp::EspPipelineLayout> m_pipeline_layout;
    std::unique_ptr<esp::EspPipeline> m_pipeline;

   public:
    MyRenderSystem() : m_device{ esp::EspRenderContext::get_device() }
    {
      /* -------------------- Configure descriptors -------------------- */
      m_descriptor_pool =
          esp::EspDescriptorPool::Builder(m_device)
              .set_max_sets(esp::EspSwapChain::MAX_FRAMES_IN_FLIGHT)
              .add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                             esp::EspSwapChain::MAX_FRAMES_IN_FLIGHT)
              .build();

      m_uniform_buffers.resize(esp::EspSwapChain::MAX_FRAMES_IN_FLIGHT);
      for (auto& buffer : m_uniform_buffers)
      {
        buffer = std::make_unique<esp::EspBuffer>(
            m_device,
            sizeof(MyUniformData),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        buffer->map();
      }

      m_descriptor_set_layout =
          esp::EspDescriptorSetLayout::Builder(m_device)
              .add_binding(0,
                           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                           VK_SHADER_STAGE_ALL_GRAPHICS)
              .build();

      m_descriptor_sets.resize(esp::EspSwapChain::MAX_FRAMES_IN_FLIGHT);
      for (int i = 0; i < m_descriptor_sets.size(); i++)
      {
        auto buffer_info     = m_uniform_buffers[i]->descriptor_info();
        m_descriptor_sets[i] = std::make_unique<esp::EspDescriptorSet>();
        esp::EspDescriptorWriter(*m_descriptor_set_layout, *m_descriptor_pool)
            .write_buffer(0, &buffer_info)
            .build(*m_descriptor_sets[i]);
      }

      /* -------------------- Create pipeline -------------------- */
      esp::PipelineConfigInfo pipeline_config{};
      esp::EspPipeline::default_pipeline_config_info(pipeline_config);

      esp::EspPipeline::Builder builder{ esp::EspRenderContext::get_device() };
      builder.set_vert_shader_path("../shaders/shader.vert.spv")
          .set_frag_shader_path("../shaders/shader.frag.spv");

      m_pipeline_layout = builder.build_pipeline_layout(
          pipeline_config,
          m_descriptor_set_layout->get_layout(),
          esp::EspPushConstantData::create_range<MyPushConstantData>());
      m_pipeline = builder.build_pipeline(
          pipeline_config,
          esp::EspRenderContext::get_scheduler().get_swap_chain_render_pass());
    }

    ~MyRenderSystem()
    {
      m_pipeline_layout->destroy(esp::EspRenderContext::get_device());
    }

    MyRenderSystem(const MyRenderSystem&)            = delete;
    MyRenderSystem& operator=(const MyRenderSystem&) = delete;

    void render(VkCommandBuffer command_buffer,
                MyGameObject& game_object,
                esp::renderer::Camera& camera)
    {
      m_pipeline->bind(command_buffer);

      /* -------------------- Set uniform data -------------------- */
      MyUniformData uniform{};
      uniform.camera_projection = camera.get_projection();

      int frame_index =
          esp::EspRenderContext::get_scheduler().get_current_frame_index();
      m_descriptor_sets[frame_index]->bind(command_buffer,
                                           m_pipeline_layout->get_layout());
      m_uniform_buffers[frame_index]->write_to_buffer(&uniform);
      m_uniform_buffers[frame_index]->flush();

      /* -------------------- Set push data -------------------- */
      MyPushConstantData push{};
      push.transform = game_object.m_transform.get_transform_mat();

      push.bind<MyPushConstantData>(command_buffer,
                                    m_pipeline_layout->get_layout());

      /* -------------------- Bind & Draw -------------------- */
      game_object.m_model->bind(command_buffer);
      game_object.m_model->draw(command_buffer);
    }
  };
} // namespace my_game

#endif // ESPERT_SANDBOX__MY_RENDER_SYSTEM_H_
