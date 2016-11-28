#ifndef MESH_H
#define MESH_H

#include <vector>
#include "ModelTexture.h"
#include <glm/mat4x4.hpp>
namespace _3DM
{
	struct ModelTexture;
	struct Mesh
	{
		std::vector <glm::vec3>		vertices;
		std::vector <glm::vec3>		normals;

		std::vector <glm::vec2>		uvs;
		std::vector <uint32_t>		indices;

		std::vector<ModelTexture>	textures;

		glm::mat4 baseModelMatrix;

		std::string name;


		uint32_t	vertexArrayObject;
		uint32_t	vertexBufferObject;
		uint32_t	elementBufferObject;
		uint32_t	uvBufferObject;
		uint32_t	normalBufferObject;


		unsigned short  diffuseIndex = 0;
		unsigned short  specularIndex = 0;
		unsigned short  normalsIndex = 0;

	};
}
#endif
