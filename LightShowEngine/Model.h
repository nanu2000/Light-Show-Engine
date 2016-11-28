#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "3DM_Info.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ShaderBase.h"
#include "Locator.h"
#include "Transform.h"
#include "Component.h"
#include "ModelSerialization.h"
#include "ModelBase.h"

namespace _3DM
{
	class _3DM_IO;
	class Model : public Component<Model>, public ModelBase
	{		
	public:
		Model(const std::string & path);
		Model() {}

		void		renderSingleMesh	(unsigned int index, ShaderBase & shader);
		int			getMeshIndex		(const std::string& MeshName) const;
		void		initialize			(ShaderBase& shader);
		void		renderAll			(ShaderBase & shader);
		glm::mat4	getMeshMatrix		(unsigned int index) const;
		void		setMeshMatrix		(unsigned int index, const glm::mat4 & newMatrix);
		unsigned int amountOfMeshes()	{ return meshes.size(); }

		std::vector<glm::vec3>* getMeshVertices	(unsigned int index);
		
		std::vector<uint32_t>* getMeshIndices(unsigned int index);


	private:
				
		std::vector <Mesh>	meshes;
		
		void initializeBuffers	(_3DM::Mesh & mesh, ShaderBase & shader);

		void initializeTexture	(_3DM::Mesh & mesh, ShaderBase & shader);

		void renderMesh			(unsigned int index, ShaderBase & shader);

		std::string			rootPath;
		bool				modelLoaded = false;
		
		friend class		_3DM::_3DM_IO;
	};
}

#endif
