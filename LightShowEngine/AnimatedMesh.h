#ifndef ANIMATED_MESH_H
#define ANIMATED_MESH_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec4.hpp>
#include "Mesh.h"

namespace _3DM
{
	struct AnimatedMesh
	{

		//these variables need serialization.

		std::vector <glm::vec4>		weights;
		std::vector <glm::vec4>		boneIDs;

		Mesh mesh;


		//These variables do not need serialization

		uint32_t	weightsBufferObject;
		uint32_t	boneIDsBufferObject;
		


	};

}
#endif
