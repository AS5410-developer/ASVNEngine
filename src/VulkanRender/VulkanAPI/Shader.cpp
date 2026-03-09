#include "Shader.hpp"

#include <fstream>

Shader::Shader(Device dev, Swapchain swapchain, const char* filename,
               VkCullModeFlags cullMode)
    : Dev(dev), Swapch(swapchain), CullMode(cullMode) {
  std::ifstream file(SHADER_MOD_LOCATION + std::string(filename),
                     std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    MainEngine->GetConsole()
        << "Failed to open file: " << SHADER_MOD_LOCATION << filename
        << "\nReason: " << std::strerror(errno) << AS::Engine::EndLine;
    return;
  }
  unsigned int Size = file.tellg();
  char* Data = new char[Size];
  file.seekg(0);
  file.read(Data, Size);
  file.close();
  if (!Data) return;

  auto shaderModuleInfo = VkShaderModuleCreateInfo{
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .pNext = 0,
      .codeSize = Size,
      .pCode = reinterpret_cast<uint32_t*>(Data)};
  ShaderModuleInfo = shaderModuleInfo;
  vkCreateShaderModule(Dev.GetDevice(), &ShaderModuleInfo, 0, &Module);

  RebuildPipeline();
}

void Shader::RebuildPipeline() {
  VkPipelineShaderStageCreateInfo psscVInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = Module,
      .pName = "VertexMain"};
  VkPipelineShaderStageCreateInfo psscFInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = Module,
      .pName = "FragmentMain"};
  VkPipelineShaderStageCreateInfo psscInfo[] = {psscVInfo, psscFInfo};

  VkVertexInputBindingDescription vibDesc{
      .binding = 0,
      .stride = sizeof(Vertex),
      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX};
  VkVertexInputAttributeDescription viaDesc[] = {
      {.location = 0,
       .binding = 0,
       .format = VK_FORMAT_R32G32_SFLOAT,
       .offset = offsetof(Vertex, pos)},
      {.location = 1,
       .binding = 0,
       .format = VK_FORMAT_R32G32_SFLOAT,
       .offset = offsetof(Vertex, texCoord)}};

  VkPipelineDepthStencilStateCreateInfo pdsdcInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .depthTestEnable = VK_FALSE};

  VkPipelineVertexInputStateCreateInfo pviscInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &vibDesc,
      .vertexAttributeDescriptionCount = 2,
      .pVertexAttributeDescriptions = viaDesc};
  VkPipelineInputAssemblyStateCreateInfo piascInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
  VkPipelineViewportStateCreateInfo pvscInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .viewportCount = 1,
      .pViewports = {},
      .scissorCount = 1,
      .pScissors = {}};
  VkPipelineRasterizationStateCreateInfo prscInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .cullMode = CullMode,
      .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
      .depthBiasConstantFactor = 0.0f,
      .depthBiasClamp = 0.0f,
      .depthBiasSlopeFactor = 0.0f,
      .lineWidth = 1.0f};
  VkPipelineMultisampleStateCreateInfo pmscInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .sampleShadingEnable = VK_FALSE};
  VkPipelineColorBlendAttachmentState pcbaState{
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
  if (Blending) {
    pcbaState.blendEnable = VK_TRUE;
    pcbaState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    pcbaState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    pcbaState.colorBlendOp = VK_BLEND_OP_ADD;
    pcbaState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    pcbaState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    pcbaState.alphaBlendOp = VK_BLEND_OP_ADD;
  }
  VkPipelineColorBlendStateCreateInfo pcbscInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments = &pcbaState};

  std::vector dynamicStates{VK_DYNAMIC_STATE_SCISSOR,
                            VK_DYNAMIC_STATE_VIEWPORT};

  VkPipelineDynamicStateCreateInfo pdscInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .dynamicStateCount = static_cast<unsigned int>(dynamicStates.size()),
      .pDynamicStates = dynamicStates.data()};

  std::vector<VkDescriptorSetLayoutBinding> ubBinding{
      {0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL, nullptr},
      {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_ALL,
       nullptr}};

  VkDescriptorSetLayoutCreateInfo layoutInfo{
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
      .pNext = 0,
      .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
      .bindingCount = static_cast<unsigned int>(ubBinding.size()),
      .pBindings = ubBinding.data()};
  vkCreateDescriptorSetLayout(Dev.GetDevice(), &layoutInfo, 0,
                              &DescriptorSetLayout);

  std::array poolSize{
      VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1},
      VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}};
  VkDescriptorPoolCreateInfo dpcInfo{
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .pNext = 0,
      .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT |
               VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
      .maxSets = 1,
      .poolSizeCount = static_cast<unsigned int>(poolSize.size()),
      .pPoolSizes = poolSize.data()};

  vkCreateDescriptorPool(Dev.GetDevice(), &dpcInfo, nullptr, &DescriptorPool);

  VkDescriptorSetAllocateInfo dsaInfo{
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      .pNext = 0,
      .descriptorPool = DescriptorPool,
      .descriptorSetCount = 1,
      .pSetLayouts = &DescriptorSetLayout};

  vkAllocateDescriptorSets(Dev.GetDevice(), &dsaInfo, &DescriptorSet);

  std::vector<VkPushConstantRange> pushRange;

  VkPipelineLayoutCreateInfo plInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .setLayoutCount = 1,
      .pSetLayouts = &DescriptorSetLayout,
      .pushConstantRangeCount = static_cast<unsigned int>(pushRange.size()),
      .pPushConstantRanges = pushRange.data()};
  auto format = Swapch.GetCurrentFormat();

  vkCreatePipelineLayout(Dev.GetDevice(), &plInfo, 0, &PipelineLayout);

  VkPipelineRenderingCreateInfo prcInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
      .pNext = 0,
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = &format,
      .depthAttachmentFormat = VK_FORMAT_UNDEFINED};

  VkGraphicsPipelineCreateInfo gpcInfo{
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .pNext = &prcInfo,
      .flags = 0,
      .stageCount = 2,
      .pStages = psscInfo,
      .pVertexInputState = &pviscInfo,
      .pInputAssemblyState = &piascInfo,
      .pViewportState = &pvscInfo,
      .pRasterizationState = &prscInfo,
      .pMultisampleState = &pmscInfo,
      .pDepthStencilState = &pdsdcInfo,
      .pColorBlendState = &pcbscInfo,
      .pDynamicState = &pdscInfo,
      .layout = PipelineLayout,
      .renderPass = nullptr};

  vkCreateGraphicsPipelines(Dev.GetDevice(), nullptr, 1, &gpcInfo, 0,
                            &Pipeline);
}
void Shader::Release() {
  vkDestroyPipelineLayout(Dev.GetDevice(), PipelineLayout, nullptr);
  vkDestroyPipeline(Dev.GetDevice(), Pipeline, nullptr);
  vkDestroyDescriptorSetLayout(Dev.GetDevice(), DescriptorSetLayout, nullptr);
}