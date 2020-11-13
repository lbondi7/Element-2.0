#pragma once

#include "ElementConstants.h"
#include "Transform.h"
#include "Entity.h"

namespace Element {

	struct VknPipeline;
	struct Mesh;
	struct Texture;

	class Model : public Entity
	{
	public:
		//explicit Model();
		//explicit Model(Pipeline* pipeline) {};
		///Model(Pipeline* pipeline, uint32_t imageCount) {};
		virtual ~Model() = default;

		Model& operator=(const Model& other) {
			this->mesh = other.mesh;
			this->texture = other.texture;
			this->transform = other.transform;
			this->pipeline = other.pipeline;
			return *this;
		}

		[[nodiscard]] virtual Mesh* GetMesh() = 0;
		[[nodiscard]] virtual Texture* GetTexture() = 0;
		[[nodiscard]] virtual VknPipeline* GetPipeline() = 0;

//        [[nodiscard]] virtual const Mesh* GetMesh() const noexcept = 0;
//        [[nodiscard]] virtual const Texture* GetTexture() const noexcept = 0;
//        [[nodiscard]] virtual const Pipeline* GetPipeline() const noexcept = 0;
//        [[nodiscard]] virtual const Pipeline* GetOldPipeline() const noexcept = 0;

        virtual void SetMesh(Mesh* mesh) = 0;
        virtual void SetTexture(Texture* texture) = 0;
        virtual void SetPipeline(VknPipeline* pipeline) = 0;

		void SetTransform(const Transform& transform);
        [[nodiscard]] const Transform& GetTransform() const noexcept;
		Transform& GetTransform() noexcept;

        [[nodiscard]] const Vec3& Position() const noexcept;
		[[nodiscard]] const Vec3& Rotation() const noexcept;
		[[nodiscard]] const Vec3& Scale() const noexcept;


	protected:
        Transform transform;
        Mesh *mesh;
        Texture *texture;
        VknPipeline *pipeline;
	};
}