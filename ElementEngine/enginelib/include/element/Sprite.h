#pragma once

#include "ElementConstants.h"

namespace Element {

	class Mesh;
	class VknPipeline;
	class Texture;

	class Sprite
	{
	public:
		Sprite() = default;
		//Sprite(Pipeline* pipeline, Mesh* mesh);
		virtual ~Sprite() = default;

		bool operator==(const Sprite& other) {
			return this->m_mesh == other.m_mesh &&
				this->m_texture == other.m_texture &&
				this->m_pipeline == other.m_pipeline;
		}

		virtual Texture* GetTexture() noexcept = 0;
		virtual VknPipeline* GetPipeline() noexcept = 0;

		virtual const Mesh* GetMesh() const = 0;
		virtual const Texture* GetTexture() const noexcept = 0;
		virtual const VknPipeline* GetPipeline() const noexcept = 0;

		virtual void SetTexture(Texture* texture, bool keepSize = true) = 0;
		virtual void SetPipeline(VknPipeline* pipeline) = 0;
		virtual const VknPipeline* GetOldPipeline() const noexcept = 0;
		//virtual void setEntityState(EntityState state) = 0;
		//virtual EntityState getEntityState() = 0;


		void SetTransform(const Transform2D& other);

		const Transform2D& GetTransform() const noexcept;
		Transform2D& GetTransform() noexcept;

		uint32_t getId() const;

	protected:

		Mesh* m_mesh;
		Texture* m_texture;
		VknPipeline* m_pipeline;
		Transform2D transform;
		bool screenSpace = true;
		uint32_t id;

	};
}