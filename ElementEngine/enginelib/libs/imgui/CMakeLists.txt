cmake_minimum_required(VERSION 2.8.12)
project(imgui)


MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(subdir ${SUBDIRS})
  #add_subdirectory(${subdir})
  add_library(${PROJECT_NAME}
      ${subdir}/imgui.cpp
      ${subdir}/imgui.h
      ${subdir}/imgui_demo.cpp
      ${subdir}/imgui_draw.cpp
      ${subdir}/imgui_internal.h
      ${subdir}/imgui_widgets.cpp
      ${subdir}/imstb_rectpack.h
      ${subdir}/imstb_textedit.h
      ${subdir}/imstb_truetype.h)

  target_include_directories(
      ${PROJECT_NAME} SYSTEM PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
  message(${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
ENDFOREACH()


#[[
add_library(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h)

target_include_directories(
    ${PROJECT_NAME} SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}")]]