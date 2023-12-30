/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/vulkan/vk_pipeline_system.h"
#include "types/vulkan/vk_pipeline_system_t.h"

#include "types/core/renderer_t.h"
#include "types/vulkan/vk_renderer_system_t.h"
#include "utils/utils.h"

#include <stdlib.h>

// See https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkGraphicsPipelineCreateInfo.html
typedef struct __GraphicsPipelineConfig {
    uint32_t shader_stage_count;
    VkPipelineShaderStageCreateInfo *shader_stages;
    VkPipelineVertexInputStateCreateInfo vertex_input_info;
    VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
    VkPipelineTessellationStateCreateInfo tessellation_info;
    VkPipelineViewportStateCreateInfo viewport_info;
    VkPipelineRasterizationStateCreateInfo rasterizer_info;
    VkPipelineMultisampleStateCreateInfo multisample_info;
    VkPipelineDepthStencilStateCreateInfo depth_stencil_info;
    VkPipelineColorBlendStateCreateInfo colour_blend_info;
    VkDynamicState *dynamic_states;
    VkPipelineDynamicStateCreateInfo dynamic_state_info;
} __GraphicsPipelineConfig;


static __GraphicsPipelineConfig __ConfigureGraphicsPipeline(const TL_PipelineDescriptor_t descriptor);

static VkPipeline __CreateGraphicsPipeline(const TLVK_FuncSet_t *devfs, const VkDevice device, const __GraphicsPipelineConfig config);


TLVK_PipelineSystem_t *TLVK_PipelineSystemCreate(const TLVK_RendererSystem_t *const renderer_system, const TL_PipelineDescriptor_t descriptor) {
    if (!renderer_system) {
        return NULL;
    }

    const TLVK_FuncSet_t *devfs = &(renderer_system->devfs);

    const TL_Debugger_t *debugger = renderer_system->renderer->debugger;
    const VkDevice device = renderer_system->vk_logical_device;

    TLVK_PipelineSystem_t *pipeline_system = malloc(sizeof(TLVK_PipelineSystem_t));
    if (!pipeline_system) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_PipelineSystemCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated memory for Vulkan pipeline system at %p", pipeline_system);

    pipeline_system->renderer_system = renderer_system;

    VkPipeline pso;
    switch (descriptor.type) {
        case TL_PIPELINE_TYPE_GRAPHICS:
            pso = __CreateGraphicsPipeline(devfs, device, __ConfigureGraphicsPipeline(descriptor));
            break;
        default:
            TL_Error(debugger, "When creating Vulkan pipeline system: pipeline descriptor specified invalid pipeline type %d", descriptor.type);
            goto outerr;
    }

    if (pso == VK_NULL_HANDLE) {
        TL_Error(debugger, "Failed to create Vulkan pipeline object for pipeline system at %p", pipeline_system);
        goto outerr;
    }

    return pipeline_system;
outerr:
    free(pipeline_system);
    return NULL;
}

void TLVK_PipelineSystemDestroy(TLVK_PipelineSystem_t *const pipeline_system) {
    if (!pipeline_system) {
        return;
    }

    const TLVK_RendererSystem_t *renderersys = pipeline_system->renderer_system;
    const TLVK_FuncSet_t *devfs = &(renderersys->devfs);
    const VkDevice device = renderersys->vk_logical_device;

    devfs->vkDestroyPipeline(device, pipeline_system->pso, NULL);

    free(pipeline_system);
}


static __GraphicsPipelineConfig __ConfigureGraphicsPipeline(const TL_PipelineDescriptor_t descriptor) {
    __GraphicsPipelineConfig config = { 0 };

    // TODO: shader modules
    config.shader_stage_count = 0;
    config.shader_stages = NULL;

    // TODO: vertex bindings and attributes
    config.vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    config.vertex_input_info.vertexBindingDescriptionCount = 0;
    config.vertex_input_info.pVertexBindingDescriptions = NULL;
    config.vertex_input_info.vertexAttributeDescriptionCount = 0;
    config.vertex_input_info.pVertexAttributeDescriptions = NULL;

    // if topology is specified, then remove the leftmost bit to convert to Vulkan type - if not, default to triangle list
    VkPrimitiveTopology topology =
        (descriptor.primitive_topology) ? descriptor.primitive_topology & ~0x80 : VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // if a strip topology is specified, then enable primitive restarts - if none specified, default to false.
    VkBool32 primitive_restart =
        (descriptor.primitive_topology & 0x80) ? VK_TRUE : VK_FALSE;
    config.input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    config.input_assembly_info.topology = topology;
    config.input_assembly_info.primitiveRestartEnable = primitive_restart;

    // TODO: tessellation patches support
    config.tessellation_info.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    config.tessellation_info.patchControlPoints = 0;

    config.viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    if (!descriptor.viewports) {
        config.viewport_info.viewportCount = 1;
        config.viewport_info.pViewports = NULL;
    } else {
        config.viewport_info.viewportCount = descriptor.viewport_count;
        config.viewport_info.pViewports = (VkViewport *) descriptor.viewports;
    }
    if (!descriptor.scissors) {
        config.viewport_info.scissorCount = 1;
        config.viewport_info.pScissors = NULL;
    } else {
        config.viewport_info.scissorCount = descriptor.scissor_count;
        config.viewport_info.pScissors = (VkRect2D *) descriptor.scissors;
    }

    config.rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    config.rasterizer_info.depthClampEnable = descriptor.rasterizer.depth_clamp;
    config.rasterizer_info.rasterizerDiscardEnable = descriptor.rasterizer.rasterizer_discard;
    config.rasterizer_info.polygonMode = (VkPolygonMode) descriptor.rasterizer.polygon_mode;
    config.rasterizer_info.cullMode = (VkCullModeFlags) descriptor.rasterizer.cull_modes;
    config.rasterizer_info.frontFace = (VkFrontFace) descriptor.rasterizer.clockwise_front_face;
    config.rasterizer_info.depthBiasEnable = descriptor.rasterizer.depth_bias;
    config.rasterizer_info.depthBiasConstantFactor = descriptor.rasterizer.depth_bias_constant_factor;
    config.rasterizer_info.depthBiasClamp = descriptor.rasterizer.depth_bias_clamp;
    config.rasterizer_info.depthBiasSlopeFactor = descriptor.rasterizer.depth_bias_slope_factor;
    config.rasterizer_info.lineWidth = descriptor.rasterizer.line_width;

    // TODO: when render passes are implemented, get the sample count from the render pass and use that
    config.multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    config.multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    config.multisample_info.sampleShadingEnable = VK_FALSE;
    config.multisample_info.minSampleShading = 0.0f;
    config.multisample_info.pSampleMask = NULL;
    config.multisample_info.alphaToCoverageEnable = VK_FALSE;
    config.multisample_info.alphaToOneEnable = VK_FALSE;

    config.depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    config.depth_stencil_info.depthTestEnable = descriptor.depth_test.test_enabled;
    config.depth_stencil_info.depthWriteEnable = descriptor.depth_test.write_enabled;
    config.depth_stencil_info.depthCompareOp = (VkCompareOp) descriptor.depth_test.compare_op;
    config.depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
    config.depth_stencil_info.minDepthBounds = 0.0f;
    config.depth_stencil_info.maxDepthBounds = 1.0f;
    // TODO: stencil test configuration
    config.depth_stencil_info.stencilTestEnable = VK_FALSE;
    config.depth_stencil_info.front = (VkStencilOpState) { 0 };
    config.depth_stencil_info.back = (VkStencilOpState) { 0 };

    // TODO: colour blend state and attachments
    config.colour_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    // config.colour_blend_info.logicOpEnable =
    // config.colour_blend_info.logicOp =
    // config.colour_blend_info.attachmentCount =
    // config.colour_blend_info.pAttachments =
    // config.colour_blend_info.blendConstants[0] =
    // config.colour_blend_info.blendConstants[1] =
    // config.colour_blend_info.blendConstants[2] =
    // config.colour_blend_info.blendConstants[3] =

    config.dynamic_states = calloc(8, sizeof(VkDynamicState)); // 8 is just arbitrary, scale this as necessary when other dynamic states are supported
    uint32_t n = 0;
    if (!config.viewport_info.pViewports)
        config.dynamic_states[n++] = VK_DYNAMIC_STATE_VIEWPORT;
    if (!config.viewport_info.pScissors)
        config.dynamic_states[n++] = VK_DYNAMIC_STATE_SCISSOR;

    config.dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    config.dynamic_state_info.pDynamicStates = config.dynamic_states;
    config.dynamic_state_info.dynamicStateCount = n;

    return config;
}

static VkPipeline __CreateGraphicsPipeline(const TLVK_FuncSet_t *devfs, const VkDevice device, const __GraphicsPipelineConfig config) {
    VkGraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = VK_NULL_HANDLE;
    pipelineInfo.flags = 0;
    pipelineInfo.stageCount = config.shader_stage_count;
    pipelineInfo.pStages = config.shader_stages;
    pipelineInfo.pVertexInputState = &config.vertex_input_info;
    pipelineInfo.pInputAssemblyState = &config.input_assembly_info;
    pipelineInfo.pTessellationState = &config.tessellation_info;
    pipelineInfo.pViewportState = &config.viewport_info;
    pipelineInfo.pRasterizationState = &config.rasterizer_info;
    pipelineInfo.pMultisampleState = &config.multisample_info;
    pipelineInfo.pDepthStencilState = &config.depth_stencil_info;
    pipelineInfo.pColorBlendState = &config.colour_blend_info;
    pipelineInfo.pDynamicState = &config.dynamic_state_info;

    // TODO: pipeline layout
    pipelineInfo.layout = VK_NULL_HANDLE;
    // TODO: render pass
    pipelineInfo.renderPass = VK_NULL_HANDLE;
    pipelineInfo.subpass = 0;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipeline pipeline;

    if (devfs->vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline)) {
        return VK_NULL_HANDLE;
    }

    return VK_NULL_HANDLE;
}
