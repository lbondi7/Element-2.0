#pragma once

#include "VknConstants.h"
#include <string>

namespace Element {

	struct TextureData {
		void* data;
		int width;
		int height;
		int channels;

		TextureData& operator=(const TextureData& other) {
			//data = new void*();
			data = new void*(other.data);
            //memcpy(data, src_data, static_cast<size_t>(m_size));
			height = other.height;
			width = other.width;
			channels = other.channels;
			return *this;
		}

	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		MeshData& operator=(const MeshData& other) {
			vertices = other.vertices;
			indices = other.indices;
			return *this;
		}

	};

	struct MaterialData{
	    glm::vec3 ambient;
	    glm::vec3 diffuse;
	    glm::vec3 specular;
	};


	namespace Resources {
    //public:
//        static Resources& Get(){
//            static Resources resources;
//            return resources;
//        }
        void LoadTextureData(const std::string& file, TextureData& textureData);
        void LoadTextureData(const char* file, TextureData& textureData);
		void LoadMeshData(const std::string& file, MeshData& meshData);
		void LoadMeshData(const char* file, MeshData& meshData);

        void LoadMaterialData(const char* file, MaterialData& materialData);

//	private:
//        Resources() = default;
//        ~Resources() = default;

	};
}