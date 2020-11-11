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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Maths/Vec3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/element/Maths/Vec2.h
    
    #main engine files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandBuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandPool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandPool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DebugRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DebugRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DebugWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Debugger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ShaderInfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptorSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DescriptorSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IdHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGUI_.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGUI_.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Inputs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LogicalDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LogicalDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PhysicalDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PhysicalDevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Pipeline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Sprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ValidationLayers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ValidationLayers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VkFunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VkFunctions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VkInitializers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VkInitializers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknConstants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknFrameBuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknFrameBuffers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknInstance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknInstance.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknPipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknPipeline.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknRenderPass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknRenderPass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknSprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknSprite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknSwapChain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknSwapChain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Transform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vec3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vec2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EleCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EleCamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Transform2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknDescriptorPool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknDescriptorPool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknResources.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VknResources.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Material.h src/Locator.cpp src/Locator.h src/Light.cpp include/element/Light.h src/LightManager.cpp src/LightManager.h)

target_include_directories(
    ${PROJECT_NAME} SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

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
