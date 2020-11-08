#pragma once

#include "ElementConstants.h"

namespace Element {

	enum class CameraType : int {
		PERSPECTIVE ,
		ORTHOGRAPHIC  = 1
	};

	class Camera
	{
	public:
		Camera();
		Camera(CameraType _type);
		~Camera()  = default;


        [[nodiscard]] const glm::vec4& GetViewport() const;
		const glm::vec4& GetRect();
		const glm::vec3& GetLookAt();
		const glm::vec3& GetForward();
		const glm::vec3& GetRight();
		const glm::vec3& GetUp();
		const glm::mat4& GetViewMatrix();
		const glm::mat4& GetProjectionMatrix();
		float GetZoom();
		float GetFOV();
		bool hasCameraChanged();

		void SetZoom(float _zoom);
		void SetFOV(float fov);
		void setCameraChanged(bool _cameraUpdated);
		void setRect(glm::vec4 _rect);
		void setViewport(glm::vec4 _viewport);

		void update(float windowWidth, float windowHeight);
		 
		void setPos(const glm::vec3& position);
		void setPos(float x, float y, float z);
		void setPos(float position);
		void setPosX(float x);
		void setPosY(float y);
		void setPosZ(float z);

		void setRot(const glm::vec3& rotation);
		void setRot(float x, float y, float z);
		void setRotX(float x);
		void setRotY(float y);
		void setRotZ(float z);

		void setScale(const glm::vec3& scale);
		void setScale(float x, float y, float z);
		void setScale(float scale);
		void setScaleX(float x);
		void setScaleY(float y);
		void setScaleZ(float z);

		const glm::vec3& getPos();
		float getPosX();
		float getPosY();
		float getPosZ();

		const glm::vec3& getRot();
		float getRotX();
		float getRotY();
		float getRotZ();

		virtual const glm::vec3& getScale();
		virtual float getScaleX();
		virtual float getScaleY();
		virtual float getScaleZ();

		bool isReady();
		void setReady(bool _ready);

	protected:

		CameraType type;
		Transform transform;
		glm::vec3 lookAtPos;
		glm::vec3 worldUp;
		glm::vec3 up;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec4 viewport;
		glm::vec4 rect;
		glm::mat4 transformMatrix  = glm::mat4(1.0f);
		glm::mat4 viewMatrix  = glm::mat4(1.0f);
		glm::mat4 projectionMatrix  = glm::mat4(1.0f);
		glm::mat4 inverted;
		float zoom  = 1.0f;
		float FOV  = 60.0f;
		bool cameraChanged  = false;
		bool ready = false;
	};
}