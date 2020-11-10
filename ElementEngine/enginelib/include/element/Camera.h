#pragma once

#include "ElementConstants.h"
#include "Transform.h"
namespace Element {

	enum class CameraType : int {
		PERSPECTIVE ,
		ORTHOGRAPHIC  = 1
	};

	class Camera
	{
	public:
//		Camera() = default;
//		Camera(CameraType _type){};
//		~Camera()  = default;

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

		bool isReady();
		void setReady(bool _ready);

	protected:

		CameraType type;
		Transform transform;
		Vec3 lookAtPos;
		Vec3 worldUp;
		Vec3 up;
		Vec3 forward;
		Vec3 right;
		float zoom  = 1.0f;
		float FOV  = 60.0f;
		bool cameraChanged  = false;
		bool ready = false;
	};
}