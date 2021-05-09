cmake_minimum_required(VERSION 2.8.12)
project(stb)

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
            ${subdir}/stb.h
            ${subdir}/stb_image.h
            ${subdir}/stb_vorbis.c)

    target_include_directories(
            ${PROJECT_NAME} SYSTEM PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
    message(${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
ENDFOREACH()


#add_library(
##       ${CMAKE_CURRENT_SOURCE_DIR}/stb-master/stb.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/stb-master/stb_image.h
#        ${CMAKE_CURRENT_SOURCE_DIR}/stb-master/stb_vorbis.c)
#target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})