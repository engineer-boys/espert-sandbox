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
		struct MyPushConstantData : esp::EspPushConstantData
		{
			glm::mat4 transform{1.0f};
		};

		esp::EspDevice& m_device;
		std::unique_ptr<esp::EspPipelineLayout> m_pipeline_layout;
		std::unique_ptr<esp::EspPipeline> m_pipeline;

	 public:
		MyRenderSystem() : m_device{esp::EspRenderContext::get_device()}
		{
			esp::PipelineConfigInfo pipeline_config{};
			esp::EspPipeline::default_pipeline_config_info(pipeline_config);

			esp::EspPipeline::Builder builder{esp::EspRenderContext::get_device()};
			builder
				.set_vert_shader_path("../shaders/shader.vert.spv")
				.set_frag_shader_path("../shaders/shader.frag.spv");

			m_pipeline_layout = builder.build_pipeline_layout(
				pipeline_config,
				esp::EspPushConstantData::create_range<MyPushConstantData>());
			m_pipeline = builder.build_pipeline(
				pipeline_config,
				esp::EspRenderContext::get_scheduler().get_swap_chain_render_pass());
		}

		~MyRenderSystem()
		{
			m_pipeline_layout->destroy(esp::EspRenderContext::get_device());
		}

		MyRenderSystem(const MyRenderSystem&) = delete;
		MyRenderSystem& operator=(const MyRenderSystem&) = delete;

		void render(VkCommandBuffer command_buffer, MyGameObject& game_object)
		{
			m_pipeline->bind(command_buffer);

			MyPushConstantData push{};
			push.transform = game_object.m_transform.get_transform_mat();

			push.bind<MyPushConstantData>(command_buffer, m_pipeline_layout->get_layout());

			game_object.m_model->bind(command_buffer);
			game_object.m_model->draw(command_buffer);
		}
	};
}

#endif //ESPERT_SANDBOX__MY_RENDER_SYSTEM_H_
