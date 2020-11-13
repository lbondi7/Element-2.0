add_library(stb stb.h stb_image.h stb_vorbis.c)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#[[
target_include_directories(imgui
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

install(TARGETS imgui DESTINATION lib)
install(FILES imgui.h DESTINATION include)
]]