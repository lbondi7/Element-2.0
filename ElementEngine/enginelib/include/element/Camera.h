#pragma once

#include "ElementConstants.h"

#include "Entity.h"
#include "Transform.h"
#include "Maths/Vec4.h"

namespace Element {

    class VknPipeline;

	enum class ViewType : int {
		PERSPECTIVE ,
		ORTHOGRAPHIC  = 1
	};

    enum class ViewDimension : int {
        _3D = 0,
        _2D = 1
    };

	class Camera : public Entity
	{
	public:
//		Camera() = default;
//		Camera(CameraType _type){};

        //Camera(CameraType _type, VknPipeline* pipeline, uint32_t imageCount);
		virtual ~Camera() = default;

		[[nodiscard]] const Vec3& getLookAt() const;
		[[nodiscard]] const Vec3& getForward() const;
		[[nodiscard]] const Vec3& getRight() const;
		[[nodiscard]] const Vec3& getUp() const;
		[[nodiscard]] float getZoom() const;
		[[nodiscard]] float getFOV() const;
		[[nodiscard]] bool hasCameraChanged() const;

		[[maybe_unused]] void setZoom(float _zoom);
		[[maybe_unused]] void setFOV(float fov);
		[[maybe_unused]] void setCameraChanged(bool _cameraUpdated);

		[[nodiscard]] const Transform& getTransform() const;
        [[maybe_unused]] void setTransform(const Transform& _transform);

		[[maybe_unused]] void setPos(const Vec3& position);
		[[maybe_unused]] void setPos(float x, float y, float z);
		[[maybe_unused]] void setPos(float position);
		[[maybe_unused]] void setPosX(float x);
		[[maybe_unused]] void setPosY(float y);
		[[maybe_unused]] void setPosZ(float z);

		[[maybe_unused]] void setRot(const Vec3& rotation);
		[[maybe_unused]] void setRot(float x, float y, float z);
		[[maybe_unused]] void setRotX(float x);
		[[maybe_unused]] void setRotY(float y);
		[[maybe_unused]] void setRotZ(float z);

        [[nodiscard]] const Vec3& getPos() const;
        [[nodiscard]] float getPosX() const;
        [[nodiscard]] float getPosY() const;
        [[nodiscard]] float getPosZ() const;

        [[nodiscard]] const Vec3& getRot() const;
		[[nodiscard]] float getRotX() const;
		[[nodiscard]] float getRotY() const;
		[[nodiscard]] float getRotZ() const;

        [[nodiscard]] const Vec4& getViewport() const;
        [[nodiscard]] const Vec4& getRect() const;
        [[maybe_unused]] void setRect(const Vec4 &_rect);
        [[maybe_unused]] void setViewport(const Vec4 &_viewport);

		[[nodiscard]] bool isEnabled() const;
		void setEnabled(bool _enabled);

		[[nodiscard]] ViewDimension getViewDimension() const;
		[[maybe_unused]] void setViewDimension(ViewDimension _dimension);

        float getNearPlane() const;
        void setNearPlane(float nearPlane);

        float getFarPlane() const;
        void setFarPlane(float farPlane);


	protected:

		ViewType type;
		ViewDimension dimension;
		Transform transform;
		Vec3 lookAtPos;
		Vec3 worldUp;
		Vec3 up;
		Vec3 forward;
		Vec3 right;
        Vec4 viewport;
        Vec4 rect;
		float zoom  = 1.0f;
		float fov  = 60.0f;
		float nearPlane = 0.0f;
		float farPlane = 1000.0f;
        bool cameraChanged  = false;
		bool enabled = false;
	};
}