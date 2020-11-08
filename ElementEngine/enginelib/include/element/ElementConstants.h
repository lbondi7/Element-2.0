#pragma  once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <algorithm>

namespace Element {

    struct Transform {

        Transform() {
            m_position = glm::vec3(0.0f);
            m_rotation = glm::vec3(0.0f);
            m_scale = glm::vec3(1.0f);
        }


        bool operator==(const Transform& other) {
            return m_position == other.m_position &&
                m_rotation == other.m_rotation &&
                m_scale == other.m_scale;
        }

        Transform& operator=(const Transform& other)
        {
            this->m_position = other.m_position;
            this->m_rotation = other.m_rotation;
            this->m_scale = other.m_scale;
            return *this;
        }

        virtual const glm::vec3& getPosition() const { return m_position; }
        float getPositionX() { return m_position.x; }
        float getPositionY() { return m_position.y; }
        float getPositionZ() { return m_position.z; }
        virtual void setPosition(const glm::vec3& position) { updated = true; m_position = position; }
        virtual void setPosition(float x, float y, float z) final { updated = true; m_position = {x, y, z}; }
        void setPositionX(float x) { updated = true; m_position.x = x; }
        void setPositionY(float y) { updated = true; m_position.y = y; }
        virtual void setPositionZ(float z) { updated = true; m_position.z = z; }

        virtual const glm::vec3& getRotation() const { return m_rotation; }
        float getRotationX() { return m_rotation.x; }
        float getRotationY() { return m_rotation.y; }
        float getRotationZ() { return m_rotation.z; }
        virtual void setRotation(const glm::vec3& rotation) { updated = true; m_rotation = rotation; }
        virtual void setRotation(float x, float y, float z) { updated = true; m_rotation = { x, y, z }; }
        virtual void setRotationX(float x) { updated = true; m_rotation.x = x; }
        virtual void setRotationY(float y) { updated = true; m_rotation.y = y; }
        void setRotationZ(float z) { updated = true; m_rotation.z = z; }

        const glm::vec3& getScale() const { return m_scale; }
        float getScaleX() { return m_scale.x; }
        float getScaleY() { return m_scale.y; }
        float getScaleZ() { return m_scale.z; }
        virtual void setScale(const glm::vec3& scale) { updated = true; m_scale = scale; }
        virtual void setScale(float x, float y, float z) { updated = true; m_scale = { x, y, z }; }
        virtual void setScale(float scale) { updated = true; m_scale = { scale, scale, scale }; }
        void setScaleX(float x) { updated = true; m_scale.x = x; }
        void setScaleY(float y) { updated = true; m_scale.y = y; }
        virtual void setScaleZ(float z) { updated = true; m_scale.z = z; }

        const bool isUpdated() const { return updated; }

        bool updated = false;
    protected:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
    private:

    };


    struct Transform2D : public Transform {
    public:
        Transform2D() {
            m_position = glm::vec3(0.0f);
            m_size = glm::vec2(1.0f);
            m_rotation = glm::vec3(0.0f);
            m_scale = glm::vec3(1.0f);
        }

        bool operator==(const Transform2D& other) {
            return m_position == other.m_position &&
                m_size == other.m_size;
                m_rotation == other.m_rotation &&
                m_scale == other.m_scale;

        }

        Transform2D& operator=(const Transform2D& other)
        {
            this->m_position = other.m_position;
            this->m_size = other.m_size;
            this->m_rotation = other.m_rotation;
            this->m_scale = other.m_scale;
            return *this;
        }

        glm::vec2 getPosition2D() { return glm::vec2(m_position); }
        void setPosition(glm::vec2 position) { m_position = glm::vec3(position, m_position.z); }
        void setPosition(const glm::vec3& position) override { m_position = glm::vec3(position.x, position.y, std::max(1.0f, std::min(0.0f, position.z))); }
        void setPosition(float x, float y) { m_position = glm::vec3(x, y, m_position.z); }

        glm::vec2 getSize() { return m_size; }
        float getSizeX() { return m_size.x; }
        float getSizeY() { return m_size.y; }
        void setSize(glm::vec2 size) { updated = true; m_size = size; }
        void setSize(float x, float y) { updated = true; m_size = { x, y }; }
        void setSizeX(float x) { updated = true; m_size.x = x; }
        void setSizeY(float y) { updated = true; m_size.y = y; }
        
        glm::vec2 getScale2D() { return glm::vec2(m_scale); }
        void setScale(const glm::vec3& scale) override { updated = true; m_scale = { scale.x, scale.y, m_scale.z }; }
        void setScale(float x, float y, float z) override { updated = true; m_scale = { x, y, m_scale.z}; }
        void setScale(float scale) override { updated = true; m_scale = { scale, scale, m_scale.z }; }
        void setScale(glm::vec2 scale) { updated = true; m_scale = { scale.x, scale.y, m_scale.z }; }
        void setScale(float x, float y) { updated = true; m_scale = { x, y , m_scale.z}; }
        void setScaleZ(float z) override {}

        void setRotation(float rotation) { updated = true; m_rotation.z = rotation; }
        void setRotation(const glm::vec3& rotation) override { updated = true; m_rotation = { m_rotation.x, m_rotation.y, rotation.z }; }
        void setRotation(float x, float y, float z) override { updated = true; m_rotation = { m_rotation.x, m_rotation.y, z }; }
        void setRotationX(float x) override {}
        void setRotationY(float y) override {}

    private:
        glm::vec2 m_size;
    };


    enum class DescriptorType : int {
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

}