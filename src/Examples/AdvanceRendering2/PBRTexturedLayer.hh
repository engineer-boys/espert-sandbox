#ifndef LAYERS_PBR_TEXTURED_LAYER_HH
#define LAYERS_PBR_TEXTURED_LAYER_HH

// libs
#include "../ModelExample/ModelExampleUtils.hh"
#include "Espert.hh"
#include "PBR2Utils.hh"

using namespace esp;

namespace advance_rendering2_example
{
  class PBRTexturedLayer : public Layer
  {
   private:
    struct MaterialPushBlock
    {
      float roughness;
      float metallic;
      float r, g, b;
    } m_params;

    struct UBOMatrices
    {
      glm::mat4 projection;
      glm::mat4 model;
      glm::mat4 view;
      glm::vec3 camPos;
    };

    struct
    {
      std::unique_ptr<EspRenderPlan> m_final_product_plan;
      std::shared_ptr<EspDepthBlock> m_depth_block;
      std::shared_ptr<EspBlock> m_float_block;

    } m_first_pass;

    struct
    {
      ModelParams m_model_params = { .m_position  = true,
                                     .m_color     = false,
                                     .m_normal    = true,
                                     .m_tex_coord = true,
                                     .m_bone_ids  = false,
                                     .m_weights   = false,
                                     .m_tangent   = true };
      std::shared_ptr<Model> m_model;
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
    } m_sphere;

    struct
    {
      std::shared_ptr<EspTexture> m_lutBrdf;
      std::shared_ptr<EspTexture> m_irradiance_cube;
      std::shared_ptr<EspTexture> m_prefiltered_cube;
    } m_textures;

    struct
    {
      std::shared_ptr<EspShader> m_shader;
      std::unique_ptr<EspUniformManager> m_uniform_manager;
      std::unique_ptr<EspVertexBuffer> m_vertex_buffer;
    } m_skybox;

   private:
    FpsCamera m_camera{};
    glm::vec3 m_lights[4];

    const std::string SKYBOX_PATH = "Textures/HDRRosendalMountainSkybox/mount.hdr";

   public:
    PBRTexturedLayer()
    {
      m_camera.set_perspective(EspWorkOrchestrator::get_swap_chain_extent_aspect_ratio());
      m_camera.set_position(glm::vec3(0.0f, 10.0f, 0.0f));
      m_camera.look_at({ 10.0f, 10, 0.0f });
      m_camera.set_move_speed(4.0f);
      m_camera.set_sensitivity(1.0f);

      m_params.r         = 0.955008f;
      m_params.g         = 0.637427f;
      m_params.b         = 0.538163f;
      m_params.roughness = 0.1f;
      m_params.metallic  = 1.0f;

      // TEXTURES
      {
        m_textures.m_lutBrdf          = generate_BRDFLUt();
        m_textures.m_irradiance_cube  = generate_irradiance_cube();
        m_textures.m_prefiltered_cube = generate_prefiltered_cube();
      }

      // first pass
      {
        m_first_pass.m_depth_block =
            EspDepthBlock::build(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                 EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                 EspImageUsageFlag::ESP_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

        m_first_pass.m_final_product_plan = EspRenderPlan::create_final({ 0.025f, 0.025f, 0.025f });
        m_first_pass.m_final_product_plan->add_depth_block(std::shared_ptr{ m_first_pass.m_depth_block });
        m_first_pass.m_final_product_plan->build();

        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_ALL_STAGES, sizeof(UBOMatrices));
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_FRAG_STAGE, 4 * sizeof(glm::vec3));
        /* IBL environment textures */
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        /* Material textures */
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_sphere.m_shader = ShaderSystem::acquire("Shaders/AdvanceRendering2/PBRTexturedExample/shader_pbr_textured");
        m_sphere.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
        m_sphere.m_shader->set_vertex_layouts({ m_sphere.m_model_params.get_vertex_layouts() });
        m_sphere.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_sphere.m_shader->build_worker();

        m_sphere.m_uniform_manager = m_sphere.m_shader->create_uniform_manager();
        {
          m_sphere.m_uniform_manager->load_texture(0, 2, m_textures.m_irradiance_cube);
          m_sphere.m_uniform_manager->load_texture(0, 3, m_textures.m_lutBrdf);
          m_sphere.m_uniform_manager->load_texture(0, 4, m_textures.m_prefiltered_cube);

          auto texture_albedo    = TextureSystem::acquire("Textures/TorHammer/albedo.png", {});
          auto texture_normal    = TextureSystem::acquire("Textures/TorHammer/normals.png", {});
          auto texture_ao        = TextureSystem::acquire("Textures/TorHammer/ao.png", {});
          auto texture_metalic   = TextureSystem::acquire("Textures/TorHammer/metallic.png", {});
          auto texture_roughness = TextureSystem::acquire("Textures/TorHammer/roughness.png", {});

          m_sphere.m_uniform_manager->load_texture(0, 5, texture_albedo);
          m_sphere.m_uniform_manager->load_texture(0, 6, texture_normal);
          m_sphere.m_uniform_manager->load_texture(0, 7, texture_ao);
          m_sphere.m_uniform_manager->load_texture(0, 8, texture_metalic);
          m_sphere.m_uniform_manager->load_texture(0, 9, texture_roughness);
        }
        m_sphere.m_uniform_manager->build();

        m_sphere.m_model = std::make_shared<Model>("Models/torhammer/TorHammer.obj", m_sphere.m_model_params);
      }

      // skybox
      {
        auto uniform_meta_data = EspUniformMetaData::create();
        uniform_meta_data->establish_descriptor_set();
        uniform_meta_data->add_buffer_uniform(EspUniformShaderStage::ESP_VTX_STAGE, sizeof(VP_PBR_Uniform2));
        uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

        m_skybox.m_shader = ShaderSystem::acquire("Shaders/AdvanceRendering2/PBRTexturedExample/skybox");
        m_skybox.m_shader->enable_depth_test(EspDepthBlockFormat::ESP_FORMAT_D32_SFLOAT,
                                             EspCompareOp::ESP_COMPARE_OP_LESS_OR_EQUAL);
        m_skybox.m_shader->set_vertex_layouts({ VTX_LAYOUT(sizeof(glm::vec3),
                                                           0,
                                                           ESP_VERTEX_INPUT_RATE_VERTEX,
                                                           ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, 0), ) });
        m_skybox.m_shader->set_worker_layout(std::move(uniform_meta_data));
        m_skybox.m_shader->build_worker();

        m_skybox.m_uniform_manager = m_skybox.m_shader->create_uniform_manager();
        m_skybox.m_uniform_manager->load_texture(0, 1, TextureSystem::acquire_cubemap(SKYBOX_PATH));
        m_skybox.m_uniform_manager->build();

        m_skybox.m_vertex_buffer =
            EspVertexBuffer::create(skybox_vertices2.data(), sizeof(glm::vec3), skybox_vertices2.size());
      }

      // UPDATE LIGHTS
      {
        const float p = 15.0f;
        m_lights[0]   = glm::vec4(-p, -p * 0.5f, -p, 1.0f);
        m_lights[1]   = glm::vec4(-p, -p * 0.5f, p, 1.0f);
        m_lights[2]   = glm::vec4(p, -p * 0.5f, p, 1.0f);
        m_lights[3]   = glm::vec4(p, -p * 0.5f, -p, 1.0f);

        m_sphere.m_uniform_manager->set_buffer_uniform(0, 1, 0, 4 * sizeof(glm::vec3), m_lights);
      }
    }

    std::shared_ptr<EspTexture> generate_BRDFLUt()
    {
      uint32_t width  = 512;
      uint32_t height = 512;

      auto uniform_meta_data = EspUniformMetaData::create();

      auto shader = ShaderSystem::acquire("Shaders/AdvanceRendering2/PBRTexturedExample/shader_brdflut");
      shader->set_attachment_formats({ EspBlockFormat::ESP_FORMAT_R16G16_SFLOAT });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->build_worker();

      auto block = EspBlock::build(EspBlockFormat::ESP_FORMAT_R16G16_SFLOAT,
                                   EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                   width,
                                   height,
                                   { 0.0f, 0.0f, 0.0f });

      auto cb_id = EspCommandBuffer::begin_only_once();

      auto render_plan = EspRenderPlan::create();
      render_plan->add_block(std::shared_ptr{ block });
      render_plan->set_command_buffer(cb_id.get());
      render_plan->build();

      auto viewport        = EspViewport();
      viewport.m_height    = width;
      viewport.m_width     = height;
      viewport.m_min_depth = 0;
      viewport.m_max_depth = 1;

      EspScissorRect scissor_rect = {};
      scissor_rect.m_width        = width;
      scissor_rect.m_height       = height;

      shader->set_scissors(cb_id.get(), scissor_rect);
      shader->set_viewport(cb_id.get(), viewport);

      render_plan->begin_plan();
      shader->only_attach(cb_id.get());

      EspJob::draw(cb_id.get(), 3);

      render_plan->end_plan();

      EspCommandBuffer::end_only_once(std::move(cb_id));

      return EspBlock::extract_texture(block);
    }

    std::shared_ptr<EspTexture> generate_irradiance_cube()
    {
      // Pipeline layout
      struct PushBlock
      {
        glm::mat4 mvp;
        // Sampling deltas
        float deltaPhi   = (2.0f * float(M_PI)) / 180.0f;
        float deltaTheta = (0.5f * float(M_PI)) / 64.0f;
      } pushBlock;

      glm::mat4 captureProjection = glm::perspective((float)(M_PI / 2.0), 1.0f, 0.1f, 10.0f);
      glm::mat4 captureViews[]    = {
        // POSITIVE_X
        glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                    glm::radians(180.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)),
        // NEGATIVE_X
        glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                    glm::radians(180.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)),
        // POSITIVE_Y
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // NEGATIVE_Y
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // POSITIVE_Z
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // NEGATIVE_Z
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
      };

      glm::mat4 vp[] = { captureProjection * captureViews[0], captureProjection * captureViews[1],
                         captureProjection * captureViews[2], captureProjection * captureViews[3],
                         captureProjection * captureViews[4], captureProjection * captureViews[5] };

      auto params       = EspRawTextureParams();
      params.as_cubemap = true;
      params.format     = EspTextureFormat::ESP_FORMAT_R32G32B32A32_SFLOAT;
      params.width      = 64;
      params.height     = 64;
      params.mip_levels = 1;

      auto my_texture = EspTexture::create_raw_texture(params);

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_ALL_STAGES, 0, sizeof(PushBlock));
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto shader = ShaderSystem::acquire("Shaders/AdvanceRendering2/PBRTexturedExample/shader_irradiance_cub");
      shader->set_attachment_formats({ EspBlockFormat::ESP_FORMAT_R32G32B32A32_SFLOAT });
      shader->set_vertex_layouts({ VTX_LAYOUT(sizeof(glm::vec3),
                                              0,
                                              ESP_VERTEX_INPUT_RATE_VERTEX,
                                              ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, 0), ) });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->build_worker();

      auto uniform_manager = shader->create_uniform_manager();
      uniform_manager->load_texture(0, 0, TextureSystem::acquire_cubemap(SKYBOX_PATH));
      uniform_manager->build();

      auto vertex_buffer = EspVertexBuffer::create(skybox_vertices2.data(), sizeof(glm::vec3), skybox_vertices2.size());

      auto block = EspBlock::build(EspBlockFormat::ESP_FORMAT_R32G32B32A32_SFLOAT,
                                   EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                   params.width,
                                   params.width,
                                   { 0.1f, 0.2f, 0.3f });

      auto cb_id = EspCommandBuffer::begin_only_once();

      auto render_plan = EspRenderPlan::create();
      render_plan->add_block(std::shared_ptr{ block });
      render_plan->set_new_layout(EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
      render_plan->set_command_buffer(cb_id.get());
      render_plan->build();

      auto viewport        = EspViewport();
      viewport.m_height    = params.width;
      viewport.m_width     = params.width;
      viewport.m_min_depth = 0;
      viewport.m_max_depth = 1;

      EspScissorRect scissor_rect = {};
      scissor_rect.m_width        = params.width;
      scissor_rect.m_height       = params.height;

      shader->set_scissors(cb_id.get(), scissor_rect);
      shader->set_viewport(cb_id.get(), viewport);

      EspImageSubresourceRange subresource_range = {};
      subresource_range.m_base_mip_level         = 0;
      subresource_range.m_level_count            = 1;
      subresource_range.m_layer_count            = 6;

      EspJob::transform_image_layout(cb_id.get(),
                                     my_texture,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_UNDEFINED,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     subresource_range);

      for (uint32_t f = 0; f < 6; f++)
      {
        render_plan->begin_plan();
        shader->only_attach(cb_id.get());

        pushBlock.mvp = vp[f];
        uniform_manager->update_push_uniform(cb_id.get(), 0, &pushBlock);
        uniform_manager->attach(cb_id.get());

        vertex_buffer->attach(cb_id.get());
        EspJob::draw(cb_id.get(), skybox_vertices2.size());

        render_plan->end_plan();

        EspImageCopy copy_region                       = {};
        copy_region.src_subresource.m_base_array_layer = 0;
        copy_region.src_subresource.m_mip_level        = 0;
        copy_region.src_subresource.m_layer_count      = 1;

        copy_region.dst_subresource.m_base_array_layer = f;
        copy_region.dst_subresource.m_mip_level        = 0;
        copy_region.dst_subresource.m_layer_count      = 1;

        copy_region.m_height = viewport.m_height;
        copy_region.m_width  = viewport.m_width;

        EspJob::copy_image(cb_id.get(),
                           block->use_as_texture(),
                           EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           my_texture,
                           EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           copy_region);
      }

      EspJob::transform_image_layout(cb_id.get(),
                                     my_texture,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                     subresource_range);

      EspCommandBuffer::end_only_once(std::move(cb_id));

      return my_texture;
    }

    std::shared_ptr<EspTexture> generate_prefiltered_cube()
    {
      glm::mat4 captureProjection = glm::perspective((float)(M_PI / 2.0), 1.0f, 0.1f, 10.0f);
      glm::mat4 captureViews[]    = {
        // POSITIVE_X
        glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                    glm::radians(180.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)),
        // NEGATIVE_X
        glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                    glm::radians(180.0f),
                    glm::vec3(1.0f, 0.0f, 0.0f)),
        // POSITIVE_Y
        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // NEGATIVE_Y
        glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // POSITIVE_Z
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // NEGATIVE_Z
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
      };

      glm::mat4 mvps[] = {
        captureProjection * captureViews[0], captureProjection * captureViews[1], captureProjection * captureViews[2],
        captureProjection * captureViews[3], captureProjection * captureViews[4], captureProjection * captureViews[5],
      };

      auto params       = EspRawTextureParams();
      params.as_cubemap = true;
      params.format     = EspTextureFormat::ESP_FORMAT_R16G16B16A16_SFLOAT;
      params.width      = 512;
      params.height     = 512;
      params.mip_levels = static_cast<uint32_t>(floor(log2(params.width))) + 1;

      auto my_texture = EspTexture::create_raw_texture(params);

      struct PushBlock
      {
        glm::mat4 mvp;
        float roughness;
        uint32_t numSamples = 32u;
      } push_block;

      auto uniform_meta_data = EspUniformMetaData::create();
      uniform_meta_data->establish_descriptor_set();
      uniform_meta_data->add_push_uniform(EspUniformShaderStage::ESP_ALL_STAGES, 0, sizeof(PushBlock));
      uniform_meta_data->add_texture_uniform(EspUniformShaderStage::ESP_FRAG_STAGE);

      auto shader = ShaderSystem::acquire("Shaders/AdvanceRendering2/PBRTexturedExample/shader_prefilter_env_map");
      shader->set_attachment_formats({ EspBlockFormat::ESP_FORMAT_R16G16B16A16_SFLOAT });
      shader->set_vertex_layouts({ VTX_LAYOUT(sizeof(glm::vec3),
                                              0,
                                              ESP_VERTEX_INPUT_RATE_VERTEX,
                                              ATTR(0, EspAttrFormat::ESP_FORMAT_R32G32B32_SFLOAT, 0), ) });
      shader->set_worker_layout(std::move(uniform_meta_data));
      shader->build_worker();

      auto uniform_manager = shader->create_uniform_manager();
      uniform_manager->load_texture(0, 0, TextureSystem::acquire_cubemap(SKYBOX_PATH));
      uniform_manager->build();

      auto vertex_buffer = EspVertexBuffer::create(skybox_vertices2.data(), sizeof(glm::vec3), skybox_vertices2.size());

      auto block = EspBlock::build(EspBlockFormat::ESP_FORMAT_R16G16B16A16_SFLOAT,
                                   EspSampleCountFlag::ESP_SAMPLE_COUNT_1_BIT,
                                   params.width,
                                   params.height,
                                   { 0.1f, 0.2f, 0.3f });

      auto cb_id = EspCommandBuffer::begin_only_once();

      auto render_plan = EspRenderPlan::create();
      render_plan->add_block(std::shared_ptr{ block });
      render_plan->set_new_layout(EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
      render_plan->set_command_buffer(cb_id.get());
      render_plan->build();

      auto viewport        = EspViewport();
      viewport.m_height    = params.width;
      viewport.m_width     = params.height;
      viewport.m_min_depth = 0;
      viewport.m_max_depth = 1;

      EspScissorRect scissor_rect = {};
      scissor_rect.m_width        = params.width;
      scissor_rect.m_height       = params.height;

      shader->set_scissors(cb_id.get(), scissor_rect);
      shader->set_viewport(cb_id.get(), viewport);

      EspImageSubresourceRange subresource_range = {};
      subresource_range.m_base_mip_level         = 0;
      subresource_range.m_level_count            = params.mip_levels;
      subresource_range.m_layer_count            = 6;

      EspJob::transform_image_layout(cb_id.get(),
                                     my_texture,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_UNDEFINED,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     subresource_range);

      for (uint32_t m = 0; m < params.mip_levels; m++)
      {
        push_block.roughness = static_cast<float>(m) / static_cast<float>(params.mip_levels - 1);
        for (uint32_t f = 0; f < 6; f++)
        {
          viewport.m_width  = static_cast<float>(params.width * std::pow(0.5f, m));
          viewport.m_height = static_cast<float>(params.height * std::pow(0.5f, m));
          shader->set_viewport(cb_id.get(), viewport);

          render_plan->begin_plan();
          shader->only_attach(cb_id.get());

          push_block.mvp = mvps[f];
          uniform_manager->update_push_uniform(cb_id.get(), 0, &push_block);
          uniform_manager->attach(cb_id.get());

          vertex_buffer->attach(cb_id.get());
          EspJob::draw(cb_id.get(), skybox_vertices2.size());

          render_plan->end_plan();

          EspImageCopy copy_region                       = {};
          copy_region.src_subresource.m_base_array_layer = 0;
          copy_region.src_subresource.m_mip_level        = 0;
          copy_region.src_subresource.m_layer_count      = 1;

          copy_region.dst_subresource.m_base_array_layer = f;
          copy_region.dst_subresource.m_mip_level        = m;
          copy_region.dst_subresource.m_layer_count      = 1;

          copy_region.m_height = viewport.m_height;
          copy_region.m_width  = viewport.m_width;

          EspJob::copy_image(cb_id.get(),
                             block->use_as_texture(),
                             EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                             my_texture,
                             EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                             copy_region);
        }
      }

      EspJob::transform_image_layout(cb_id.get(),
                                     my_texture,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     EspImageLayout::ESP_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                     subresource_range);

      EspCommandBuffer::end_only_once(std::move(cb_id));

      return my_texture;
    }

   private:
    virtual void update(float dt) override
    {
      Scene::set_current_camera(&m_camera);

      m_first_pass.m_final_product_plan->begin_plan();
      {
        /* Rendering model */
        {
          m_sphere.m_shader->attach();

          UBOMatrices mvpc = {};
          mvpc.model       = glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1, 0, 0));
          mvpc.model       = glm::scale(mvpc.model, glm::vec3(10.0f, 10.0f, 10.0f));
          mvpc.view        = m_camera.get_view();
          mvpc.projection  = m_camera.get_projection();
          mvpc.camPos      = m_camera.get_position();
          m_sphere.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(UBOMatrices), &mvpc);
          m_sphere.m_uniform_manager->attach();

          for (auto node_info : *(m_sphere.m_model))
          {
            for (const auto& mesh_idx : node_info.m_current_node->m_meshes)
            {
              auto& mesh = m_sphere.m_model->m_meshes[mesh_idx];
              EspJob::draw_indexed(mesh.m_index_count, 1, mesh.m_first_index);
            }
          }
        }

        /* Rendering skybox */
        {
          m_skybox.m_shader->attach();
          m_skybox.m_vertex_buffer->attach();

          VP_PBR_Uniform2 vp{};
          vp.view = glm::mat4(glm::mat3(m_camera.get_view()));
          vp.proj = m_camera.get_projection();
          m_skybox.m_uniform_manager->update_buffer_uniform(0, 0, 0, sizeof(VP_PBR_Uniform2), &vp);
          m_skybox.m_uniform_manager->attach();

          EspJob::draw(skybox_vertices2.size());
        }
      }
      m_first_pass.m_final_product_plan->end_plan();

      m_first_pass.m_depth_block->clear();
    }
  };
} // namespace advance_rendering2_example

#endif // LAYERS_PBR_TEXTURED_LAYER_HH
