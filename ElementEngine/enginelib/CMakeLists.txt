project(ElementEngine)

add_library(
    ${PROJECT_NAME}
    STATIC
    
    #front facing includes
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Camera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/DebugWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Debugger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/ElementConstants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Engine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/GameSettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/IdHandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/InputIndices.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Inputs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Model.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Sprite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Time.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Transform2D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Light.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Entity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Maths/Vec3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Maths/Vec2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Maths/Vec4.h
    
    #main engine files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VknBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VknBuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Camera/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/CommandPool/VknCommandBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/CommandPool/VknCommandBuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/CommandPool/VknCommandPool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/CommandPool/VknCommandPool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Debug/DebugRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Debug/DebugRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Debug/DebugWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Debug/Debugger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader/ShaderInfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/VknDescriptorSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/VknDescriptorSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Device/Device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Device/Device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/IdHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGUI_/ImGUI_.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGUI_/ImGUI_.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VknImage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VknImage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Inputs/Inputs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Device/LogicalDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Device/LogicalDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resource/Mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resource/Mesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entities/Model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Device/PhysicalDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Device/PhysicalDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/Pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/Pipeline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/PipelineManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/PipelineManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/PipelineData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resource/Resources.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resource/Resources.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader/Shader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entities/Sprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture/Texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture/Texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Utilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Utilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Debug/ValidationLayers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Debug/ValidationLayers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VkFunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VkFunctions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VkInitializers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VkInitializers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VknConstants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/RenderPass/VknFrameBuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/RenderPass/VknFrameBuffers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VknInstance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/VknInstance.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entities/VknModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entities/VknModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/VknPipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/VknPipeline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/RenderPass/VknRenderPass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/RenderPass/VknRenderPass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/VknRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/VknRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entities/VknSprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entities/VknSprite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/SwapChain/VknSwapChain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/SwapChain/VknSwapChain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/GlfwWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/GlfwWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Transform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Maths/Vec3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Maths/Vec2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Camera/EleCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Camera/EleCamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Transform2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/VknDescriptorPool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan/Pipeline/VknDescriptorPool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resource/Material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resource/Material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Locator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Misc/Locator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Lighting/Light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Lighting/LightManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Lighting/LightManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Maths/Vec4.cpp
    src/Vulkan/Allocator/Allocator.cpp src/Vulkan/Allocator/Allocator.h src/Vulkan/Allocator/MemoryAllocator.cpp src/Vulkan/Allocator/MemoryAllocator.h src/Vulkan/Allocator/BufferAllocator.cpp src/Vulkan/Allocator/BufferAllocator.h)

#target_include_directories(
#    ${PROJECT_NAME} SYSTEM PUBLIC
#    "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/libs/glm"
#    "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb"
#    "${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyobjloader"
#    "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui"
#)

target_include_directories(
    ${PROJECT_NAME} SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(
    ${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Vulkan")

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_subdirectory(libs/tinyobjloader)
add_subdirectory(libs/stb)
add_subdirectory(libs/imgui)

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC stb)
target_link_libraries(${PROJECT_NAME} PUBLIC tinyobjloader)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

