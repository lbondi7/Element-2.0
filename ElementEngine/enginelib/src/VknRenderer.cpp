#include "VknRenderer.h"

#include "VkInitializers.h"
#include "DefaultResources.h"
#include <element/Utilities.h>
#include <element/GameSettings.h>
#include <element/Debugger.h>
#include <element/Inputs.h>

#include <imgui/imgui.h>

#include <stdexcept>
#include <memory>

Element::VknRenderer::VknRenderer()
{
    camera = nullptr;
}

Element::VknRenderer::~VknRenderer()
{

}

void Element::VknRenderer::createSurface() {

    if (glfwCreateWindowSurface(instance->GetVkInstance(), window->GetGLFWWindow(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void Element::VknRenderer::destroySurface()
{
    vkDestroySurfaceKHR(instance->GetVkInstance(), surface, nullptr);
}

void Element::VknRenderer::init()
{
    window = std::make_unique<VknWindow>();

    instance = std::make_unique<Instance>();

    createSurface();
    debugLayers.setupDebugMessenger(instance->GetVkInstance());

    Device::setupPhysicalDevice(instance->GetVkInstance(), surface);
    Device::setupLogicalDevice(surface);

    //Resources::LoadMaterialData("");

    DefaultResources::init();
    std::vector<std::string> meshNames{ "cube.obj", "quad.obj", "error.obj" };
    for (const auto& mesh : meshNames)
    {
        std::string name = Element::Utilities::extractName(mesh);
        meshes.emplace(name, std::make_unique<Mesh>());
        meshes[name]->LoadMesh(mesh);
    }

    std::vector<std::string> texts{ "default.jpg", "texture.jpg" };
    for (const auto& texture : texts)
    {
        loadTexture(texture);
    }

    createRenderer();

    const auto& device = Device::GetPhysicalDevice()->GetSelectedDevice();
    Debugger::Get().log("Heap Size", static_cast<float>(device.m_memoryProperties.memoryHeaps->size));
    Debugger::Get().log("Heap Count", static_cast<float>(device.m_memoryProperties.memoryHeapCount));
    Debugger::Get().log("Heap Index", static_cast<float>(device.m_memoryProperties.memoryTypes->heapIndex));
    Debugger::Get().log("Type Count", static_cast<float>(device.m_memoryProperties.memoryTypeCount));
}

void Element::VknRenderer::deInit()
{
    cleanupRenderer();

    for (const auto& model : meshes)
    {
        model.second->Destroy();
    }
    for (const auto& model : texs)
    {
        model.second->destroy();
    }
    DefaultResources::deinit();

    debugRenderer->deint();

    Device::destroy();

    destroySurface();

    debugLayers.Destroy();
}

void Element::VknRenderer::beginFrame()
{
    if (renderStage != RenderStage::START)
        return;

    VkResult result = swapChain->AquireNextImage(graphicsSemaphores[currentFrame], inFlightFences[currentFrame]);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateRenderer();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    renderStage = RenderStage::PRESENT_READY;
}

void Element::VknRenderer::renderFrame()
{
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2(window->getSize().x, window->getSize().y);

    auto mousePos = Inputs::Get().getCursorPos();
    io.MousePos = ImVec2(mousePos.x, mousePos.y);
    //Debugger::Get().log("Mouse X", io.MousePos.x);
    //Debugger::Get().log("Mouse y", io.MousePos.y);
    io.MouseDown[0] = Inputs::Get().buttonDown(Element::MOUSE_BUTTON::LEFT) || Inputs::Get().buttonHeld(Element::MOUSE_BUTTON::LEFT);
    io.MouseDown[1] = Inputs::Get().buttonDown(Element::MOUSE_BUTTON::RIGHT);
    CheckModels();
    CheckSprites();

    //if (rebuildCmdBuffers || debugRenderer->isInvalid())
        rebuildCommandBuffers();

    camera->update(window->getSize().x, window->getSize().y);
    updateUniformBuffers();
    camera->setCameraChanged(false);
}

void Element::VknRenderer::endFrame()
{
    if (renderStage != RenderStage::PRESENT_READY)
        return;

    if (imagesInFlight[swapChain->CurrentImageIndex()] != VK_NULL_HANDLE) {
        vkWaitForFences(Device::getVkDevice(), 1, &imagesInFlight[swapChain->CurrentImageIndex()], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[swapChain->CurrentImageIndex()] = inFlightFences[currentFrame];

    commandBuffers[swapChain->CurrentImageIndex()]->Submit(graphicsSemaphores[currentFrame], presentSemaphores[currentFrame], inFlightFences[currentFrame]);

    VkResult result = swapChain->Present(presentSemaphores[currentFrame]);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->getFramebufferResized()) {
        recreateRenderer();
        window->setFramebufferResized(false);
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    renderStage = RenderStage::START;
}

void Element::VknRenderer::recreateRenderer() {

    if (window->tooSmall())
        return;

    cleanupSwapChain();

    swapChain->init(window->GetGLFWWindow(), surface);
    renderPass->init(swapChain.get());
    m_pipelineManager->regeneratePipelines(swapChain.get(), renderPass.get());
    camera->setCameraChanged(true);

    for (const auto& model : vknModels)
        model->reInit(swapChain->getImageCount());

    for (const auto& sprite : vknSprites)
        sprite->reInit(swapChain->getImageCount());

    CheckModels();
    CheckSprites();
    rebuildCommandBuffers();

    renderStage = RenderStage::START;
}

void Element::VknRenderer::createRenderer() {

    swapChain = std::make_unique<SwapChain>(window->GetGLFWWindow(), surface);
    renderPass = std::make_unique<RenderPass>(swapChain.get());
    m_shaderManager = std::make_unique<ShaderManager>();

    PipelineData pipelineData{};
    pipelineData.descriptorInfo.push_back({ DescriptorType::STATIC_UNIFORM_BUFFER, Shader::ShaderType::VERTEX, 0 });
    pipelineData.descriptorInfo.push_back({ DescriptorType::IMAGE, Shader::ShaderType::FRAGMENT, 1 });
    pipelineData.shaders = { m_shaderManager->GetShader(Shader::ShaderType::VERTEX, "shader"), m_shaderManager->GetShader(Shader::ShaderType::FRAGMENT, "shader") };
    m_pipelineManager = std::make_unique<PipelineManager>(swapChain.get(), renderPass.get(), &pipelineData);

    pipelineData.descriptorInfo.pop_back();
    pipelineData.shaders = { m_shaderManager->GetShader(Shader::ShaderType::VERTEX, "shader2"), m_shaderManager->GetShader(Shader::ShaderType::FRAGMENT, "shader2") };
    m_pipelineManager->generatePipeline("red", &pipelineData);

    graphicsCommandPool = std::make_unique<CommandPool>();
    commandBuffers.resize(swapChain->getImageCount());
    for (auto& commandBuffer : commandBuffers)
        commandBuffer = std::make_unique<CommandBuffer>(graphicsCommandPool.get(), false);

    debugRenderer = std::make_unique<DebugRenderer>();
    debugRenderer->init(renderPass->GetVkRenderPass());

    rebuildCommandBuffers();
    createSyncObjects();
}

void Element::VknRenderer::signalExit()
{
    window->close();
}

void Element::VknRenderer::cleanupSwapChain() {

    vkDeviceWaitIdle(Device::getVkDevice());

    swapChain->DestroyDepthResource();
    swapChain->DestroyColourResource();

    m_pipelineManager->flush();
    renderPass->Destroy();

    swapChain->Destroy();
}

void Element::VknRenderer::cleanupRenderer() {
    cleanupSwapChain();

    const auto& logicalDevice = Device::getVkDevice();

    for (const auto& model : vknModels)
        model->destroy();

    for (const auto& sprite : vknSprites)
        sprite->destroy();

    for (auto& commandBuffer : commandBuffers) {
        commandBuffer->destroy();
    }

    m_pipelineManager->destroy();
    m_shaderManager->Destroy();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(logicalDevice, presentSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice, graphicsSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
    }

    graphicsCommandPool->destroyVkCommandPool();
}

void Element::VknRenderer::rebuildCommandBuffers() {

    vkDeviceWaitIdle(Device::getVkDevice());
    //Debugger::Get().startTimer("commandBuffers");
    debugRenderer->update();

    for (size_t i = 0; i < commandBuffers.size(); i++) {

        commandBuffers[i]->begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

        const auto& vkCmdBuffer = commandBuffers[i]->GetVkCommandBuffer();
        renderPass->begin(vkCmdBuffer, static_cast<int>(i));

        VkViewport viewport{};
        VkRect2D scissorRect{};
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        if (!camera)
        {
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(window->getSize().x);
            viewport.height = static_cast<float>(window->getSize().y);

            scissorRect.offset = { 0, 0 };
            scissorRect.extent = { swapChain->Extent().width,  swapChain->Extent().height };
        }
        else
        {
            float xy = window->getSize().x;
            float yx = camera->GetViewport().x;
            viewport.x = yx * xy;
            viewport.y = camera->GetViewport().y * window->getSize().y;
            viewport.width = static_cast<float>(camera->GetViewport().z * window->getSize().x);
            viewport.height = static_cast<float>(camera->GetViewport().w * window->getSize().y);

            scissorRect.offset = { static_cast<int>(camera->GetRect().x * window->getSize().x),
                                   static_cast<int>(camera->GetRect().y * window->getSize().y) };
            scissorRect.extent = { static_cast<uint32_t>(camera->GetRect().z * window->getSize().x),
                                   static_cast<uint32_t>(camera->GetRect().w * window->getSize().y) };
        }

        vkCmdSetViewport(vkCmdBuffer, 0, 1, &viewport);
        vkCmdSetScissor(vkCmdBuffer, 0, 1, &scissorRect);

        int j = 0;
        for (auto& model : vknModels)
        {
            if (!model || model->getEntityState() != EntityState::RENDERED)
                continue;

            m_pipelineManager->BindPipeline(model->GetPipeline(), vkCmdBuffer);
            vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, model->GetPipeline()->GetVkPipelineLayout(), 0, 1,
                                    &model->descriptorSet->descriptorSets[i], 0, nullptr);
            BindMesh(model->GetMesh(), vkCmdBuffer);
            vkCmdDrawIndexed(vkCmdBuffer, static_cast<uint32_t>(model->GetMesh()->data.indices.size()), 1, 0, 0, 0);

            ++j;
        }
        j = 0;

        for (auto& sprite : vknSprites)
        {
            if (!sprite || sprite->getEntityState() != EntityState::RENDERED)
                continue;

            m_pipelineManager->BindPipeline(sprite->GetPipeline(), vkCmdBuffer);
            vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, sprite->GetPipeline()->GetVkPipelineLayout(), 0, 1, &sprite->descriptorSet->descriptorSets[i], 0, nullptr);
            BindMesh(sprite->GetMesh(), vkCmdBuffer);
            vkCmdDrawIndexed(vkCmdBuffer, static_cast<uint32_t>(sprite->GetMesh()->data.indices.size()), 1, 0, 0, 0);
            ++j;
        }

        UnbindMeshes();
        m_pipelineManager->UnbindPipelines();

        debugRenderer->draw(vkCmdBuffer);

        renderPass->end(vkCmdBuffer);

        commandBuffers[i]->end();
    }
    rebuildCmdBuffers = false;

    //Debugger::Get().endTimer("commandBuffers");
}

void Element::VknRenderer::BindMesh(Element::Mesh* mesh, VkCommandBuffer commandBuffer) {

    if (mesh->bound)
        return;

    UnbindMeshes();
    VkDeviceSize offsets[] = { 0 };
    mesh->bind(commandBuffer, offsets);
}

void Element::VknRenderer::UnbindMeshes() {
    for (auto& mesh : meshes)
        mesh.second->bound = false;
}

void Element::VknRenderer::createSyncObjects() {
    graphicsSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    presentSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChain->getImageCount(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    const auto& logicalDevice = Device::getVkDevice();
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &graphicsSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &presentSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void Element::VknRenderer::renderModel(Model* model)
{
    if (model && dynamic_cast<VknModel*>(model))
        dynamic_cast<VknModel*>(model)->setEntityState(EntityState::RENDERED);
}

void Element::VknRenderer::renderSprite(Sprite* sprite)
{
    if(sprite && dynamic_cast<VknSprite*>(sprite))
        dynamic_cast<VknSprite*>(sprite)->setEntityState(EntityState::RENDERED);
}

void Element::VknRenderer::CheckModels()
{
    for (const auto& model : vknModels)
    {
        if (!model || model->getEntityState() == EntityState::DESTROY)
            continue;

        if (model->isDirty() != DirtyFlags::CLEAN || model->getEntityState() != model->getPrevEntityState())
            rebuildCmdBuffers = true;
    }
}

void Element::VknRenderer::CheckSprites()
{
    for (const auto& sprite: vknSprites)
    {
        if (!sprite || sprite->getEntityState() == EntityState::DESTROY)
            continue;

        if (sprite->isDirty() != DirtyFlags::CLEAN || sprite->getEntityState() != sprite->getPrevEntityState())
            rebuildCmdBuffers = true;
    }
}

void Element::VknRenderer::updateUniformBuffers()
{
    //Debugger::Get().startTimer("uniformBuffers");
    for (auto& model : vknModels)
    {
        if (!model)
            continue;

        model->updateUniformBuffers(camera->hasCameraChanged(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), swapChain->CurrentImageIndex());
        model->setDirty(DirtyFlags::CLEAN);
        model->setEntityState(model->getEntityState() == EntityState::RENDERED ? EntityState::READY_TO_RENDER : EntityState::NOT_RENDERED);
    }

    for (auto& sprite : vknSprites)
    {
        if (!sprite)
            continue;

        sprite->updateUniformBuffers(camera->hasCameraChanged(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), swapChain->CurrentImageIndex());
        sprite->setDirty(DirtyFlags::CLEAN);
        sprite->setEntityState(sprite->getEntityState() == EntityState::RENDERED ? EntityState::READY_TO_RENDER : EntityState::NOT_RENDERED);
    }
   // Debugger::Get().endTimer("uniformBuffers");
}

Element::Mesh* Element::VknRenderer::getMesh(const std::string& name)
{
    return meshes[name].get();
}

Element::Texture* Element::VknRenderer::getTexture(const std::string& name)
{
    return texs[name].get();
}

//Element::VknPipeline* Element::VknRenderer::getPipeline(const std::string& name)
//{
//    return m_pipelineManager->getPipeline(name);
//}

Element::Window* Element::VknRenderer::getWindow()
{
    return window.get();
}

Element::Model* Element::VknRenderer::createModel()
{
    auto& model = vknModels.emplace_back();
    model = std::make_unique<VknModel>(m_pipelineManager->getDefaultPipeline(), swapChain->getImageCount());
    return dynamic_cast<Model*>(model.get());
    //return new VknModel(m_pipelineManager->getDefaultPipeline());
}

Element::Sprite* Element::VknRenderer::createNewSprite()
{
    auto& sprite = vknSprites.emplace_back();
    sprite = std::make_unique<VknSprite>(m_pipelineManager->getDefaultPipeline(), meshes["quad"].get(), swapChain->getImageCount());
    return dynamic_cast<Sprite*>(sprite.get());
   // return new VknSprite(m_pipelineManager->getDefaultPipeline(), models["quad"].get(), swapChain->getImageCount());
}

Element::Camera* Element::VknRenderer::createCamera(Element::CameraType type)
{
    return new Camera(type);
}

void Element::VknRenderer::loadTexture(const std::string& texture)
{
    std::string name = Element::Utilities::extractName(texture);
    texs.emplace(name, std::make_unique<Texture>());
    texs[name]->Load(texture);
}

void Element::VknRenderer::setClearColour(const glm::vec3& clearCol)
{
    renderPass->setClearColour(clearCol);
    window->setFramebufferResized(true);
}

void Element::VknRenderer::setClearColourNormalised(const glm::vec3& clearCol)
{
    glm::vec3 temp = clearCol;

    temp.r = (temp.r - 0.0f) / (255.0f - 0.0f);
    temp.g = (temp.g - 0.0f) / (255.0f - 0.0f);
    temp.b = (temp.b - 0.0f) / (255.0f - 0.0f);

    renderPass->setClearColour(temp);
    window->setFramebufferResized(true);
}
