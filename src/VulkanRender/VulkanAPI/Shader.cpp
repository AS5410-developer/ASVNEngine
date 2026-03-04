#include "Shader.hpp"

#include "Depth.hpp"
#include "filesystem.h"
#include "glm.hpp"

extern Depth* MainDepth;

Shader::Shader() {}
Shader::Shader(Device* dev, Swapchain* swapchain, const char* filename,
               bool enableDepthTest, vk::CullModeFlags cullMode)
    : Dev(dev),
      Swapch(swapchain),
      EnableDepthTest(enableDepthTest),
      CullMode(cullMode) {
  auto fs = Engine::GetInstance().GetEngineFuncs()->fsapi;

  auto shader = new std::string(fs->Gamedir());
  const char* path =
      shader->append("/").append(SHADER_MOD_LOCATION).append(filename).c_str();

  if (!fs->SysFileExists(path)) {
    return;
  }

  auto ShaderFile = fs->Open(path, "rb", false);

  if (!ShaderFile) {
    return;
  }
  fs->Seek(ShaderFile, 0, SEEK_END);
  const size_t Size = fs->Tell(ShaderFile);
  if (Size == -1) {
    return;
  }

  char* Data = new char[Size];

  fs->Seek(ShaderFile, 0, 0);

  fs->Read(ShaderFile, Data, Size);

  fs->Close(ShaderFile);

  delete path;

  auto shaderModuleInfo = vk::ShaderModuleCreateInfo{
      .codeSize = Size, .pCode = reinterpret_cast<uint32_t*>(Data)};
  ShaderModuleInfo = shaderModuleInfo;
  Module = vk::raii::ShaderModule{Dev->GetDevice(), ShaderModuleInfo};

  RebuildPipeline();
}

vk::raii::Pipeline& Shader::GetPipeline() { return Pipeline; }
void Shader::SetCullMode(vk::CullModeFlags cullMode) { CullMode = cullMode; }
vk::CullModeFlags Shader::GetCullMode() { return CullMode; }
void Shader::SetBlending(bool enabled) { Blending = enabled; }
bool Shader::GetBlending() { return Blending; }

void Shader::RebuildPipeline() {
  vk::PipelineShaderStageCreateInfo psscVInfo{
      .stage = vk::ShaderStageFlagBits::eVertex,
      .module = Module,
      .pName = "VertexMain"};
  vk::PipelineShaderStageCreateInfo psscFInfo{
      .stage = vk::ShaderStageFlagBits::eFragment,
      .module = Module,
      .pName = "FragmentMain"};
  vk::PipelineShaderStageCreateInfo psscInfo[] = {psscVInfo, psscFInfo};

  vk::VertexInputBindingDescription vibDesc{
      .binding = 0,
      .stride = sizeof(Vertex),
      .inputRate = vk::VertexInputRate::eVertex};
  vk::VertexInputAttributeDescription viaDesc[] = {
      {.location = 0,
       .binding = 0,
       .format = vk::Format::eR32G32B32Sfloat,
       .offset = offsetof(Vertex, pos)},
      {.location = 1,
       .binding = 0,
       .format = vk::Format::eR32G32B32Sfloat,
       .offset = offsetof(Vertex, color)},
      {.location = 2,
       .binding = 0,
       .format = vk::Format::eR32G32Sfloat,
       .offset = offsetof(Vertex, texCoord)}};

  vk::PipelineDepthStencilStateCreateInfo pdsdcInfo{
      .depthTestEnable = vk::True,
      .depthWriteEnable = vk::True,
      .depthCompareOp = vk::CompareOp::eLess,
      .depthBoundsTestEnable = vk::False,
      .stencilTestEnable = vk::False};
  if (!EnableDepthTest) pdsdcInfo.depthTestEnable = vk::False;

  vk::PipelineVertexInputStateCreateInfo pviscInfo{
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &vibDesc,
      .vertexAttributeDescriptionCount = 3,
      .pVertexAttributeDescriptions = viaDesc};
  vk::PipelineInputAssemblyStateCreateInfo piascInfo{
      .topology = vk::PrimitiveTopology::eTriangleList};
  vk::PipelineViewportStateCreateInfo pvscInfo{.flags = {},
                                               .viewportCount = 1,
                                               .pViewports = {},
                                               .scissorCount = 1,
                                               .pScissors = {}};
  vk::PipelineRasterizationStateCreateInfo prscInfo{
      .depthClampEnable = vk::False,
      .rasterizerDiscardEnable = vk::False,
      .cullMode = CullMode,
      .frontFace = vk::FrontFace::eCounterClockwise,
      .depthBiasEnable = vk::False,
      .depthBiasConstantFactor = 0.0f,
      .depthBiasClamp = 0.0f,
      .depthBiasSlopeFactor = 0.0f,
      .lineWidth = 1.0f};
  vk::PipelineMultisampleStateCreateInfo pmscInfo{
      .rasterizationSamples = vk::SampleCountFlagBits::e1,
      .sampleShadingEnable = vk::False};
  vk::PipelineColorBlendAttachmentState pcbaState{
      .blendEnable = vk::False,
      .colorWriteMask =
          vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};
  if (Blending) {
    pcbaState.blendEnable = vk::True;
    pcbaState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    pcbaState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    pcbaState.colorBlendOp = vk::BlendOp::eAdd;
    pcbaState.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    pcbaState.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    pcbaState.alphaBlendOp = vk::BlendOp::eAdd;
  }
  vk::PipelineColorBlendStateCreateInfo pcbscInfo{.logicOpEnable = vk::False,
                                                  .logicOp = vk::LogicOp::eCopy,
                                                  .attachmentCount = 1,
                                                  .pAttachments = &pcbaState};

  std::vector dynamicStates{vk::DynamicState::eScissor,
                            vk::DynamicState::eViewport};

  vk::PipelineDynamicStateCreateInfo pdscInfo{
      .dynamicStateCount = dynamicStates.size(),
      .pDynamicStates = dynamicStates.data()};

  vk::DescriptorSetLayoutBinding ubBinding{
      0, vk::DescriptorType::eUniformBuffer, 1,
      vk::ShaderStageFlagBits::eVertex, nullptr};
  vk::DescriptorSetLayoutCreateInfo layoutInfo{.bindingCount = 1,
                                               .pBindings = &ubBinding};
  DescriptorSetLayout =
      vk::raii::DescriptorSetLayout(Dev->GetDevice(), layoutInfo);

  vk::PushConstantRange pushRange{
      .stageFlags = vk::ShaderStageFlagBits::eVertex,
      .offset = 0,
      .size = sizeof(glm::mat4)};

  vk::PipelineLayoutCreateInfo plInfo{.setLayoutCount = 0,
                                      .pushConstantRangeCount = 1,
                                      .pPushConstantRanges = &pushRange};
  auto format = Swapch->GetCurrentFormat();

  PipelineLayout = vk::raii::PipelineLayout(Dev->GetDevice(), plInfo);
  vk::PipelineRenderingCreateInfo prcInfo{
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = &format,
      .depthAttachmentFormat = MainDepth->GetFormat()};

  vk::GraphicsPipelineCreateInfo gpcInfo{.pNext = prcInfo,
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

  Pipeline = vk::raii::Pipeline(Dev->GetDevice(), nullptr, gpcInfo);
}
vk::raii::DescriptorSetLayout& Shader::GetDescriptorSetLayout() {
  return DescriptorSetLayout;
}
vk::raii::PipelineLayout& Shader::GetPipelineLayout() { return PipelineLayout; }
