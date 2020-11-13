#pragma once

#include "ElementConstants.h"

#include "Entity.h"
#include "Transform.h"
#include "Maths/Vec4.h"

namespace Element {

    class VknPipeline;

	enum class CameraType : int {
		PERSPECTIVE ,
		ORTHOGRAPHIC  = 1
	};

	class Camera : public Entity
	{
	public:
//		Camera() = default;
//		Camera(CameraType _type){};

        //Camera(CameraType _type, VknPipeline* pipeline, uint32_t imageCount);
		virtual ~Camera() = default;

		[[nodiscard]] const Vec3& GetLookAt() const;
		[[nodiscard]] const Vec3& GetForward() const;
		[[nodiscard]] const Vec3& GetRight() const;
		[[nodiscard]] const Vec3& GetUp() const;
		float GetZoom();
		float GetFOV();
		bool hasCameraChanged();

		void SetZoom(float _zoom);
		void SetFOV(float fov);
		void setCameraChanged(bool _cameraUpdated);

		[[nodiscard]] const Element::Transform& getTransform() const;
        void setTransform(const Element::Transform& _transform);


		void setPos(const Vec3& position);
		void setPos(float x, float y, float z);
		void setPos(float position);
		void setPosX(float x);
		void setPosY(float y);
		void setPosZ(float z);

		void setRot(const Vec3& rotation);
		void setRot(float x, float y, float z);
		void setRotX(float x);
		void setRotY(float y);
		void setRotZ(float z);

        [[nodiscard]] const Vec3& getPos() const;
		float getPosX();
		float getPosY();
		float getPosZ();

        [[nodiscard]] const Vec3& getRot() const;
		float getRotX();
		float getRotY();
		float getRotZ();

        [[nodiscard]] const Vec4& getViewport() const;
        [[nodiscard]] const Vec4& getRect() const;
        void setRect(const Vec4 &_rect);
        void setViewport(const Vec4 &_viewport);

		bool isEnabled() const;
		void setEnabled(bool _enabled);

	protected:

		CameraType type;
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
		bool cameraChanged  = false;
		bool enabled = false;
	};
}