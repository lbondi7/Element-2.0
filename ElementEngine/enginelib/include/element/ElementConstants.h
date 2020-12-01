#pragma  once

//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <algorithm>

namespace Element {

    enum class BindObjectType : int {
        IMAGE = 1,
        STATIC_UNIFORM_BUFFER = 6,
        STATIC_STORAGE_BUFFER = 7,
        DYNAMIC_UNIFORM_BUFFER = 8,
        DYNAMIC_STORAGE_BUFFER = 9
    };

    enum class WindowMode : int {

        FULL_SCREEN = 0,
        WINDOWED = 1,
        BORDERLESS_FULLSCREEN = 2,
        BORDERLESS_WINDOWED = 3
    };

    struct Monitor {
        int width = 0;
        int height = 0;
        int refreshRate = 0;
        int id = 0;
        const char* name;
    };

    enum class EntityState : int{
        NOT_RENDERED = 0,
        RENDERED = 1,
        READY_TO_RENDER = 2,
        DESTROY = 3
    };

    enum class ShaderType : int {
        VERTEX = 0,
        FRAGMENT = 1,
        GEOMETRY = 2,
    };

    enum class PipelinePolygonMode : int {
        FILL = 0,
        LINE = 1,
        POINT = 2,
    };

    enum class PipelineCulling : int {
        NONE = 0,
        FRONT = 1,
        BACK = 2,
        BOTH = 3
    };

}