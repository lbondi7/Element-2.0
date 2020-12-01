#include "VknRenderer.h"

#include <Misc/Locator.h>
#include <Resource/Resources.h>
#include <Vulkan/Device/Device.h>
#include <ImGUI_/ImGUI_.h>
#include <Vulkan/Allocator/Allocator.h>

#include <imgui.h>

#include <element/Debugger.h>
#include <element/Inputs.h>

#include <stdexcept>
#include <memory>

Element::VknRenderer::VknRenderer()
{

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
    window = std::make_unique<GlfwWindow>();

    Inputs::get().init(window->GetGLFWWindow());

    instance = std::make_unique<Instance>();

    createSurface();

    Device::setupPhysicalDevice(instance->GetVkInstance(), surface);
    Device::setupLogicalDevice(surface);

    Locator::initResources(std::make_unique<Resources>());
    //Locator::initVknResources(std::make_unique<VknResources>());

    createRenderer();
}

void Element::VknRenderer::deInit()
{
    cleanupRenderer();

    debugRenderer->deint();

    Allocator::Get().deallocate();

    Device::destroy();

    destroySurface();
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

    uint32_t imageIndex = swapChain->imageIndex();

    ImGui::Begin("Light");
    ImGui::SliderInt("Type", &type, 0, 2);
    ImGui::SliderFloat3("Pos", pos, -5.0f, 5.0f);
    ImGui::SliderFloat3("Col", col, 0.0f, 1.0f);
    ImGui::SliderFloat3("Atten", attenuation,  0.0f, 1.0f);
    ImGui::SliderFloat3("Dir", dir, -1.0f, 1.0f);
    ImGui::SliderFloat("Intensity", &intensity, 0.0f, 5.0f);
    ImGui::SliderFloat("Cut Off", &cutOff, 0.0f, 20.0f);
    ImGui::SliderFloat("Outer Cut Off", &outerCutOff, 0.0f, 20.0f);
    //ImGui::ShowDemoWindow();
    ImGui::End();
    ImGUI::Get().end();

    auto& light = m_lightManager->getLights()[0];
    light->setType(static_cast<LightType>(type));
    light->setPosition(pos[0],pos[1], pos[2]);
    light->setColour(col[0],col[1], col[2]);
    light->setConstant(attenuation[0]);
    light->setLinear(attenuation[1]);
    light->setQuadratic(attenuation[2]);
    light->setCutOff(cutOff);
    light->setOuterCutOff(outerCutOff);
    light->setIntensity(intensity);

    CheckModels();
    CheckSprites();
    //if (rebuildCmdBuffers)
        rebuildCommandBuffers();

    m_lightManager->update(imageIndex);

    int i = 0;
    for (auto& cam : cameras)
    {
        if(cam)
            cam->update(window->getSize().x, window->getSize().y, imageIndex);
        else
            cameras.erase(cameras.cbegin() + i);
        ++i;
    }

    updateUniformBuffers();
    //camera->setCameraChanged(false);
}

void Element::VknRenderer::endFrame()
{
    if (renderStage != RenderStage::PRESENT_READY)
        return;

    if (imagesInFlight[swapChain->imageIndex()] != VK_NULL_HANDLE) {
        vkWaitForFences(Device::getVkDevice(), 1, &imagesInFlight[swapChain->imageIndex()], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[swapChain->imageIndex()] = inFlightFences[currentFrame];

    commandBuffers[swapChain->imageIndex()]->Submit(graphicsSemaphores[currentFrame], presentSemaphores[currentFrame], inFlightFences[currentFrame]);

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
    auto imageCount = swapChain->getImageCount();
    renderPass->reInit();
    m_pipelineManager->regeneratePipelines();
    //camera->setCameraChanged(true);

    for (const auto& model : vknModels)
        model->reInit(imageCount);

    for (const auto& sprite : vknSprites)
        sprite->reInit(imageCount);

    for (const auto& cam : cameras)
        cam->reInit(imageCount);

    m_lightManager->reInit(imageCount);

    CheckModels();
    CheckSprites();
    rebuildCommandBuffers();

    renderStage = RenderStage::START;
}

void Element::VknRenderer::createRenderer() {

    swapChain = std::make_unique<VknSwapChain>(window->GetGLFWWindow(), surface);
    renderPass = std::make_unique<VknRenderPass>(swapChain.get());

    m_pipelineManager = std::make_unique<PipelineManager>(swapChain.get(), renderPass.get());

    PipelineData pipelineData{};
    ShaderInfo shaderInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::VERTEX,
                             "defaultLighting", 2, 0, 1000 };
    pipelineData.shaderInfo.emplace_back(shaderInfo);

    shaderInfo = {BindObjectType::IMAGE, ShaderType::FRAGMENT,
                  "defaultLighting", 2,1};
    pipelineData.shaderInfo.emplace_back(shaderInfo);

    shaderInfo = {BindObjectType::STATIC_UNIFORM_BUFFER, ShaderType::FRAGMENT,
                  "defaultLighting", 2, 2};
    pipelineData.shaderInfo.emplace_back(shaderInfo);

    m_pipelineManager->generatePipeline("default", pipelineData);

    graphicsCommandPool = std::make_unique<VknCommandPool>();
    commandBuffers.resize(swapChain->getImageCount());
    for (auto& commandBuffer : commandBuffers)
        commandBuffer = std::make_unique<VknCommandBuffer>(graphicsCommandPool.get(), false);

    debugRenderer = std::make_unique<DebugRenderer>();
    debugRenderer->init(swapChain.get(), renderPass.get());

    m_lightManager =std::make_unique<LightManager>(
            m_pipelineManager->getPipeline("default"), swapChain->getImageCount());

    rebuildCommandBuffers();
    createSyncObjects();
}

void Element::VknRenderer::signalExit()
{
    window->close();
}

void Element::VknRenderer::cleanupSwapChain() {

    vkDeviceWaitIdle(Device::getVkDevice());
    m_pipelineManager->flush();
    renderPass->Destroy();
    swapChain->Destroy();
}

void Element::VknRenderer::cleanupRenderer() {
    cleanupSwapChain();

    const auto& logicalDevice = Device::getVkDevice();

    for (const auto& model : vknModels) {
        if(model)
            model->destroy();
    }
    for (const auto& sprite : vknSprites) {
        if(sprite)
            sprite->destroy();
    }
    for (const auto& camera : cameras) {
        if(camera)
            camera->destroy();
    }

    vknModels.clear();
    vknSprites.clear();
    cameras.clear();

    for (auto& commandBuffer : commandBuffers) {
        commandBuffer->destroy();
    }

    m_pipelineManager->destroy();
    m_lightManager->deInit();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(logicalDevice, presentSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice, graphicsSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
    }

    graphicsCommandPool->destroyVkCommandPool();

    Locator::getResource()->destroy();
}

void Element::VknRenderer::rebuildCommandBuffers() {

    vkDeviceWaitIdle(Device::getVkDevice());
    //Debugger::Get().startTimer("commandBuffers");
    const auto& windowSize = window->getSize();
    debugRenderer->update(windowSize.x, windowSize.y);

    for (size_t i = 0; i < commandBuffers.size(); i++) {

        commandBuffers[i]->begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

        const auto& vkCmdBuffer = commandBuffers[i]->GetVkCommandBuffer();
        renderPass->begin(vkCmdBuffer, static_cast<int>(i));
        
        if (!cameras.empty()) {
            for (auto& cam :  cameras) {
                if(!cam || !cam->isEnabled()) continue;

                cam->setViewportAndRect(vkCmdBuffer, windowSize);

                std::vector<VkDescriptorSet> descriptorSets;
                if(cam->getViewDimension() == ViewDimension::_3D) {

                    for (auto &model : vknModels) {
                        if (!model || model->getEntityState() != EntityState::RENDERED)
                            continue;

                        auto pipeline = model->GetPipeline();
                        m_pipelineManager->BindPipeline(pipeline, vkCmdBuffer);

                        descriptorSets =
                                {cam->getDescriptorSet(pipeline->getName())->getDescriptorSet(i)};

                        descriptorSets.emplace_back(
                                m_lightManager->getDescriptorSet(pipeline->getName())->getDescriptorSet(i));

                        descriptorSets.emplace_back(model->getDescriptorSet()->getDescriptorSet(i));

                        vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                pipeline->GetVkPipelineLayout(), 0,
                                                static_cast<uint32_t>(descriptorSets.size()),
                                                descriptorSets.data(), 0, nullptr);

                        BindMesh(model->GetMesh(), vkCmdBuffer);
                        model->GetMesh()->draw(vkCmdBuffer);
                    }

                    debugRenderer->draw3D(vkCmdBuffer, cam->getDescriptorSet("default")->getDescriptorSet(i), i);
                }
                else if(cam->getViewDimension() == ViewDimension::_2D) {
                    for (auto &sprite : vknSprites) {
                        if (!sprite || sprite->getEntityState() != EntityState::RENDERED)
                            continue;

                        auto pipeline = sprite->GetPipeline();
                        m_pipelineManager->BindPipeline(sprite->GetPipeline(), vkCmdBuffer);

                        descriptorSets =
                                {cam->getDescriptorSet(pipeline->getName())->getDescriptorSet(i)};

                        descriptorSets.emplace_back(
                                m_lightManager->getDescriptorSet(pipeline->getName())->getDescriptorSet(i));

                        descriptorSets.emplace_back(sprite->getDescriptorSet()->getDescriptorSet(i));

                        vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                sprite->GetPipeline()->GetVkPipelineLayout(), 0,
                                                static_cast<uint32_t>(descriptorSets.size()),
                                                descriptorSets.data(), 0, nullptr);

                        BindMesh(sprite->GetMesh(), vkCmdBuffer);
                        sprite->GetMesh()->draw(vkCmdBuffer);
                    }
                }

                Locator::getResource()->unbindMeshes();
                m_pipelineManager->UnbindPipelines();
            }
        }
        debugRenderer->drawImGUI(vkCmdBuffer);
        renderPass->end(vkCmdBuffer);

        commandBuffers[i]->end();
    }
    rebuildCmdBuffers = false;

    //Debugger::Get().endTimer("commandBuffers");
}

void Element::VknRenderer::BindMesh(Element::Mesh* mesh, VkCommandBuffer commandBuffer) {

    if (mesh->bound)
        return;

    Locator::getResource()->unbindMeshes();
    mesh->bind(commandBuffer);
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

        if (model->isDirty() != DirtyFlags::CLEAN || model->getEntityState() != model->getPrevEntityState()) {
            rebuildCmdBuffers = true;
            break;
        }
    }
}

void Element::VknRenderer::CheckSprites()
{
    if(rebuildCmdBuffers)
        return;

    for (const auto& sprite: vknSprites)
    {
        if (!sprite || sprite->getEntityState() == EntityState::DESTROY)
            continue;

        if (sprite->isDirty() != DirtyFlags::CLEAN || sprite->getEntityState() != sprite->getPrevEntityState()) {
            rebuildCmdBuffers = true;
            break;
        }
    }
}

void Element::VknRenderer::updateUniformBuffers()
{
    auto imageIndex = swapChain->imageIndex();
    for (auto& model : vknModels)
    {
        if (!model)
            continue;

        model->updateUniformBuffers(imageIndex);
        model->setDirty(DirtyFlags::CLEAN);
        model->setEntityState(model->getEntityState() == EntityState::RENDERED ? EntityState::READY_TO_RENDER : EntityState::NOT_RENDERED);
    }

    for (auto& sprite : vknSprites)
    {
        if (!sprite)
            continue;

        sprite->updateUniformBuffers(imageIndex);
        sprite->setDirty(DirtyFlags::CLEAN);
        sprite->setEntityState(sprite->getEntityState() == EntityState::RENDERED ? EntityState::READY_TO_RENDER : EntityState::NOT_RENDERED);
    }
}

Element::Mesh* Element::VknRenderer::getMesh(const std::string& name)
{
    return Locator::getResource()->mesh(name);
}

Element::Texture* Element::VknRenderer::getTexture(const std::string& name)
{
    return Locator::getResource()->texture(name);
}

Element::Window* Element::VknRenderer::getWindow()
{
    return window.get();
}

Element::Model* Element::VknRenderer::createModel()
{
    auto& model = vknModels.emplace_back();
    model = std::make_unique<VknModel>(m_pipelineManager->getPipeline("default"),
                                       swapChain->getImageCount());
    return dynamic_cast<Model*>(model.get());
}

Element::Sprite* Element::VknRenderer::createNewSprite()
{
    auto& sprite = vknSprites.emplace_back();
    sprite = std::make_unique<VknSprite>(m_pipelineManager->getPipeline("default"),
                                         Locator::getResource()->mesh("quad"),
                                         swapChain->getImageCount());
    return dynamic_cast<Sprite*>(sprite.get());
}

Element::Camera* Element::VknRenderer::createCamera(Element::ViewType type, ViewDimension dimension)
{
    auto camera = new EleCamera(type, dimension, m_pipelineManager->getPipeline("default"),
                                              swapChain->getImageCount());

    addCamera(camera, -1);
    return dynamic_cast<Camera*>(camera);
}

void Element::VknRenderer::setClearColour(const Vec3& clearCol)
{
    renderPass->setClearColour(clearCol);
    window->setFramebufferResized(true);
}

void Element::VknRenderer::setClearColourNormalised(const Vec3& clearCol)
{
    auto temp = clearCol;

    temp.r = (temp.r - 0.0f) / (255.0f - 0.0f);
    temp.g = (temp.g - 0.0f) / (255.0f - 0.0f);
    temp.b = (temp.b - 0.0f) / (255.0f - 0.0f);

    renderPass->setClearColour(temp);
    window->setFramebufferResized(true);
}

void Element::VknRenderer::addCamera(Element::Camera *_camera, int element)
{
    auto c = dynamic_cast<EleCamera*>(_camera);
    if(!c)
        return;

    if(element == -1) {
        if (cameras.size() < Device::GetPhysicalDevice()->GetSelectedDevice().m_properties.limits.maxViewports)
            cameras.emplace_back(c);
        else {
            for (auto &cam : cameras) {
                if (!cam)
                    cam = c;
            }
        }
    }
    else if(element > 0 || element < cameras.size())
        cameras[element] = c;
}

std::unique_ptr<Element::Camera> Element::VknRenderer::createUniqueCamera(Element::ViewType type, ViewDimension dimension) {
    auto camera = std::make_unique<EleCamera>(type, dimension, m_pipelineManager->getPipeline("default"),
                                                swapChain->getImageCount());

    addCamera(camera.get(), -1);
    return std::move(camera);
}