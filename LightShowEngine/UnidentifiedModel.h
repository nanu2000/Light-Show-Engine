#ifndef UN_IDENTIFIED_MODEL_H
#define UN_IDENTIFIED_MODEL_H

#include "3DM_Info.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Locator.h"

#include "SkeletalSystem.h"
#include "AnimatedMesh.h"

#include <sstream>


namespace _3DM
{

	class UnidentifiedModel
	{

		virtual void initialize(const Shader& shader) = 0;
		virtual void renderAll(const Shader& shader) = 0;

		virtual void renderSingleMesh(unsigned int index, const Shader & shader) = 0;

		virtual int getMeshIndex(const std::string& MeshName) = 0;
	
	private:



	};

}


#endif // !UN_IDENTIFIED_MODEL
