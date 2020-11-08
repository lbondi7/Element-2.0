project(ElementApp)
add_executable(
        ${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Object.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Object.hpp )


target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ElementEngine)