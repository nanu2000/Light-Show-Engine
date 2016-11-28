#include "Model.h"
_3DM::Model::Model(const std::string & path)
{
	_3DM_IO modelLoader;
	*this = modelLoader.readModel(path);

	if (!modelLoaded)
	{
		printf("Please load in a model before initializing buffers. ( _3DM::Model::initialize() )\n");
		return;
	}

}

void _3DM::Model::renderSingleMesh(unsigned int index, ShaderBase & shader)
{
	if (!modelLoaded)
	{
		DBG_LOG("Please load a model before trying to render one.\n");
		return;
	}
	if (index < meshes.size())
	{
		renderMesh(index, shader);
	}
	else
	{
		DBG_LOG("This index goes out of bounds (_3DM::Model::renderSingleMesh)\n");
	}
}
//Returns -1 on failure
int _3DM::Model::getMeshIndex(const std::string & MeshName) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i].name == MeshName)
		{
			return i;
		}
	}
	return -1;
}

void _3DM::Model::initializeTexture(_3DM::Mesh & mesh, ShaderBase & shader)
{
	for (GLuint j = 0; j < mesh.textures.size(); j++)
	{
		std::stringstream stringStream;
		
		mesh.textures.at(j).imageID = TextureLocator::getService().getTexture(mesh.textures.at(j).imagePath).getTextureData();
		
		if (mesh.textures.at(j).imageType == TextureType::Diffuse)
		{
			mesh.diffuseIndex++;
			stringStream << mesh.diffuseIndex;

			mesh.textures.at(j).uniformName = Shaders::getUniformName(Shaders::UniformName::DiffuseTexture) + stringStream.str();

			continue;
		}

		if (mesh.textures.at(j).imageType == TextureType::Normals)
		{
			mesh.normalsIndex++;
			stringStream << mesh.normalsIndex;


			mesh.textures.at(j).uniformName = Shaders::getUniformName(Shaders::UniformName::NormalTexture) + stringStream.str();

			continue;
		}

		if (mesh.textures.at(j).imageType == TextureType::Specular)
		{
			mesh.specularIndex++;
			stringStream << mesh.specularIndex;

			mesh.textures.at(j).uniformName = Shaders::getUniformName(Shaders::UniformName::SpecularTexture) + stringStream.str();

			continue;
		}
	}


}

void _3DM::Model::initializeBuffers(_3DM::Mesh & mesh, ShaderBase & shader)
{

	GLint uvAttribute		= Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates);
	GLint positionAttribute = Shaders::getAttribLocation(Shaders::AttribName::Position);
	GLint normalsAttribute	= Shaders::getAttribLocation(Shaders::AttribName::Normal);


	glGenVertexArrays(1, &mesh.vertexArrayObject);
	glBindVertexArray(mesh.vertexArrayObject);


	glGenBuffers(1, &mesh.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);



	glGenBuffers(1, &mesh.elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &mesh.uvBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvs.size(), &mesh.uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(uvAttribute);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);


	glGenBuffers(1, &mesh.normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), &mesh.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(normalsAttribute);
	glVertexAttribPointer(normalsAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	

}

void _3DM::Model::initialize(ShaderBase & shader)
{

	if (!modelLoaded)
	{
		printf("Please load in a model before initializing buffers. ( _3DM::Model::initialize() )\n");
		return;
	}

	// loop through each mesh and initialize them
	for (unsigned int i = 0; i < meshes.size(); i++)
	{

		initializeTexture(meshes[i], shader);
		
		initializeBuffers(meshes[i], shader);

	}
	glBindVertexArray(0);
	animatedModel = false;

}

void _3DM::Model::renderAll(ShaderBase & shader)
{
	if (!modelLoaded)
	{
		#ifdef DEBUG
		printf("Please load a model before trying to render one.\n");
		#endif
		return;
	}
	//Loop through all of the meshes in the model
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		renderMesh(i, shader);
	}

}

glm::mat4 _3DM::Model::getMeshMatrix(unsigned int index) const
{
	if(index < meshes.size() && index >= 0)
	{
		return meshes.at(index).baseModelMatrix;
	}
	DBG_LOG("Index went out of bounds (glm::mat4 _3DM::Model::getMeshMatrix in model.cpp)\n");

	return glm::mat4();
}

void _3DM::Model::setMeshMatrix(unsigned int index, const glm::mat4 & newMatrix)
{
	if (index < meshes.size() && index >= 0)
	{
		meshes.at(index).baseModelMatrix = newMatrix;
	}
}

std::vector<glm::vec3>* _3DM::Model::getMeshVertices(unsigned int index)
{
	if(index < meshes.size())
	{
		return &meshes.at(index).vertices;
	}

	return nullptr;
}

std::vector<uint32_t>* _3DM::Model::getMeshIndices(unsigned int index)
{
	if (index < meshes.size())
	{
		return &meshes.at(index).indices;
	}

	return nullptr;
}

void _3DM::Model::renderMesh(unsigned int index, ShaderBase & shader)
{

	glBindVertexArray(meshes.at(index).vertexArrayObject);	//Bind VAO
	
	glm::mat4 transformation = meshes.at(index).baseModelMatrix;

	transformation = glm::translate	(transformation,transform.position);
	transformation = glm::rotate(transformation, glm::angle(transform.rotation), glm::axis(transform.rotation));
	transformation = glm::scale		(transformation, transform.scale);


	glUniformMatrix4fv
	(
		Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ModelMatrix),
		1,
		GL_FALSE,
		glm::value_ptr(transformation)
	);
	

	for (GLuint j = 0; j < meshes.at(index).textures.size(); j++)
	{

		glActiveTexture(GL_TEXTURE0 + j); // Activate proper texture unit before binding
		
		glUniform1i
		(
			glGetUniformLocation
			(
				shader.getProgramID(),
				meshes.at(index).textures.at(j).uniformName.c_str()
			),
			j
		);

		glBindTexture(GL_TEXTURE_2D, meshes.at(index).textures.at(j).imageID);

	}

	glDrawElements(GL_TRIANGLES, meshes.at(index).indices.size(), GL_UNSIGNED_INT, 0);	//Draw the mesh
	

	glBindVertexArray(0);
}
