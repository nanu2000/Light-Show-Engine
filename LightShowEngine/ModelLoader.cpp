#include "ModelLoader.h"
// set of all animated bones/nodes




#ifdef DEBUG
void _3DM::ModelLoader::traverseSkeleton(const BoneNode & parent)
{

	printf("\n");
	for (unsigned int j = 0; j < parent.children.size(); j++)
	{
		printf(parent.children.at(j).name.c_str());
		traverseSkeleton(parent.children.at(j));
		printf("\n");
	}
	

}
#endif

bool _3DM::ModelLoader::loadAnimatedModel(const char* filePath, AnimatedModel& model)
{
	
	Assimp::Importer	importer;	//create the assimp importer.
	const	aiScene*	scene = nullptr;

	//read the file, and get the scene pointer.
	scene = importer.ReadFile
		(
			filePath,							//the file path
			aiProcess_Triangulate			|	//triangulate faces
			aiProcess_GenSmoothNormals		|	//generate smooth normals
			//aiProcess_ValidateDataStructure |	//use data validation
			aiProcess_LimitBoneWeights		|	//limit bone weights to 4
			aiProcess_FlipUVs					//flip uvs
		);	

	//if the scene is a null pointer, then there was an error importing.
	if (!scene)
	{

		#ifdef DEBUG
		printf("Error importing ");		
		printf(importer.GetErrorString());
		#endif

		return false;
	}

	//Set the models file path
	model.rootPath = static_cast<std::string>(filePath);

	//Here we identify the root path of the file
	for (int i = model.rootPath.size() - 1; i >= 0; i--)
	{
		if (model.rootPath[i] == '/' || model.rootPath[i] == '\\')
		{
			model.rootPath = model.rootPath.substr(0, i + 1);
			i = -1;
		}
	}


	#ifdef DEBUG
	printf("Number of total meshes: "); 
	printf("%i\n", scene->mNumMeshes);
	printf("Number of total animations: "); 
	printf("%i\n", scene->mNumAnimations);
	#endif

	//Process animations
	processAnimations(*scene, model);

	//process the nodes in the mesh, starting with the root node
	processNode(*scene, *scene->mRootNode, model);

	//if the scene has animations
	if (scene->HasAnimations())
	{
		//build the bone tree
		buildBoneTree(*scene, *scene->mRootNode, model.modelsAnimation.rootBone, model);

		//update the bone tree once
		model.updateBoneTree(0, &model.modelsAnimation.rootBone,glm::mat4(1.0f));

	}
	
	#ifdef DEBUG
	traverseSkeleton(model.modelsAnimation.rootBone);
	
	printf("\n");
	#endif

	//the model has successfuly loaded.
	model.modelLoaded = true;

	return true;

}
bool _3DM::ModelLoader::loadModel(const char * filePath, Model & model)
{
	Assimp::Importer	importer;	//create the assimp importer.
	const	aiScene*	scene = nullptr;

	//read the file, and get the scene pointer.
	scene = importer.ReadFile
		(
			filePath,							//the file path
			aiProcess_Triangulate |	//triangulate faces
			aiProcess_GenSmoothNormals |	//generate smooth normals
			aiProcess_FlipUVs	|				//flip uvs
			aiProcess_Debone
			);

	//if the scene is a null pointer, then there was an error importing.
	if (!scene)
	{

		#ifdef DEBUG
		printf("Error importing ");
		printf(importer.GetErrorString());
		#endif

		return false;
	}

	//Set the models file path
	model.rootPath = static_cast<std::string>(filePath);

	//Here we identify the root path of the file
	for (int i = model.rootPath.size() - 1; i >= 0; i--)
	{
		if (model.rootPath[i] == '/' || model.rootPath[i] == '\\')
		{
			model.rootPath = model.rootPath.substr(0, i + 1);
			i = -1;
		}
	}


#ifdef DEBUG
	printf("Number of total meshes: ");
	printf("%i\n", scene->mNumMeshes);
#endif

	//process the nodes in the mesh, starting with the root node
	processNode(*scene, *scene->mRootNode, model);
	

	//the model has successfuly loaded.
	model.modelLoaded = true;

	return true;
}

/*******************************************************************************************************************************
*Used to process animations and keyframes(channels). Passed by reference because we are expecting the arguments to not be null.*
********************************************************************************************************************************/
void _3DM::ModelLoader::processAnimations(const aiScene& asmpScene, AnimatedModel& model)
{

	//Loop through the scene's animations
	for (unsigned int i = 0; i < asmpScene.mNumAnimations; i++)
	{
		//create a temporary animation
		Animation temporaryAnimation;
			
		temporaryAnimation.duration			= static_cast<float>(asmpScene.mAnimations[i]->mDuration);			//give it a duration
		temporaryAnimation.ticksPerSecond	= static_cast<float>(asmpScene.mAnimations[i]->mTicksPerSecond);	//give it the amount of ticks per second.

																												//TODO Move This
		AnimationClip newAnimationClip;

		newAnimationClip.name = "CompleteAnimation";
		newAnimationClip.startTime = 0;
		newAnimationClip.endTime = temporaryAnimation.duration;

		model.animationClips.push_back(newAnimationClip);


		#ifdef DEBUG 
		printf("Animation Indexed at | %i : ",i);
		printf(temporaryAnimation.nameOfAnimation.c_str());
		printf("\n");
		#endif

		//Loop through this animations channels
		for (unsigned int j = 0; j < asmpScene.mAnimations[i]->mNumChannels; j++)
		{
			//create a temporary channel
			Channel temporaryChannel;
			
			temporaryChannel.name = asmpScene.mAnimations[i]->mChannels[j]->mNodeName.data;
				
			for (unsigned int k = 0; k < asmpScene.mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
			{
				temporaryChannel.positionKeys.push_back
				(
					Vector3Key
					(
					static_cast<float>(asmpScene.mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime),
					glm::vec3
					(
					asmpScene.mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x,
					asmpScene.mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y,
					asmpScene.mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z
					)
					)
				);
			}

			for (unsigned int k = 0; k < asmpScene.mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)
			{
				temporaryChannel.rotationKeys.push_back
				(
					QuatKey
					(
					static_cast<float>(asmpScene.mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime),
					glm::quat
					(
					asmpScene.mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w,
					asmpScene.mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x,
					asmpScene.mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y,
					asmpScene.mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z
					)
					)
				);
			}

			for (unsigned int k = 0; k < asmpScene.mAnimations[i]->mChannels[j]->mNumScalingKeys; k++)
			{
				temporaryChannel.scalingKeys.push_back
				(
					Vector3Key
					(
					static_cast<float>(asmpScene.mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime),
					glm::vec3
					(
					asmpScene.mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x,
					asmpScene.mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y,
					asmpScene.mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z
					)
					)
				);
			}

			int boneID = 0;

			if (model.boneID.find(temporaryChannel.name) == model.boneID.end())
			{
				boneID = model.boneID.size();

				model.boneID[temporaryChannel.name] = boneID;
				
			}
			

			#ifdef DEBUG
			printf("\nBone ");
			printf(temporaryChannel.name.c_str());
			printf(" is bone ID : %i\n",boneID);
			#endif



			temporaryAnimation.channels.push_back(temporaryChannel);
		}
			
		//Loop though all of the bone transforms and set them to the identity matrix
		for (int z = 0; z < MAX_BONES; z++)
		{
			temporaryAnimation.boneTransformations.push_back(glm::mat4(1.0f));
		}

		model.modelsAnimation = temporaryAnimation;
	
}
	
	model.modelsAnimation.rootBone.name = "rootBoneTreeNode";
	
}

void _3DM::ModelLoader::processMesh(const aiScene& asmpScene, const aiNode& asmpNode, const aiMesh& asmpMesh, Model& model)
{
#ifdef DEBUG
	printf("Processing a mesh: "); printf(asmpMesh.mName.C_Str());
	printf("Mesh is not animated\n");
#endif 

	Mesh tempMesh;
		
	tempMesh.baseModelMatrix = toglmMat4(asmpNode.mTransformation);


	if (asmpNode.mParent)
	{
		tempMesh.baseModelMatrix = toglmMat4(asmpNode.mParent->mTransformation) * toglmMat4(asmpNode.mTransformation);
	}

	// cycle through each vertex in the mesh
	for (unsigned int i = 0; i < asmpMesh.mNumVertices; i++)
	{

		// load the vertices
		glm::vec3 tempV;
		glm::vec2 tempUV = glm::vec2(0, 0);

		tempV.x = asmpMesh.mVertices[i].x;
		tempV.y = asmpMesh.mVertices[i].y;
		tempV.z = asmpMesh.mVertices[i].z;
		tempMesh.vertices.push_back(tempV);

		if (asmpMesh.HasTextureCoords(0))
		{
			tempUV.x = asmpMesh.mTextureCoords[0][i].x;
			tempUV.y = asmpMesh.mTextureCoords[0][i].y;
		}

		tempMesh.uvs.push_back(tempUV);

		// load the normals (if they exist)
		if (asmpMesh.HasNormals())
		{
			glm::vec3 tempN;
			tempN.x = asmpMesh.mNormals[i].x;
			tempN.y = asmpMesh.mNormals[i].y;
			tempN.z = asmpMesh.mNormals[i].z;
			tempMesh.normals.push_back(tempN);
		}

	}

	// cycle through each face to get the indices
	for (unsigned int i = 0; i < asmpMesh.mNumFaces; i++)
	{

		for (unsigned int j = 0; j < asmpMesh.mFaces[i].mNumIndices; j++)
		{
			tempMesh.indices.push_back(asmpMesh.mFaces[i].mIndices[j]);
		}

	}

	if (asmpScene.HasMaterials())
	{
		// so that we don't have to type out that whole thing every time
		aiMaterial* mat = asmpScene.mMaterials[asmpMesh.mMaterialIndex];
		
		addNewTexture(tempMesh, model.rootPath, *mat, aiTextureType_DIFFUSE);
		addNewTexture(tempMesh, model.rootPath, *mat, aiTextureType_SPECULAR);
		addNewTexture(tempMesh, model.rootPath, *mat, aiTextureType_NORMALS);
		
	}

	model.meshes.push_back(tempMesh);
}

void _3DM::ModelLoader::processNode(const aiScene& asmpScene,const aiNode& asmpNode, AnimatedModel& model)
{

	#ifdef DEBUG
	printf("processing node "); 
	printf(asmpNode.mName.C_Str()); 
	printf("\n");
	#endif

	// cycle through each mesh within this node
	if (asmpNode.mNumMeshes > 0)
	{
		// cycle through each mesh
		for (unsigned int i = 0; i < asmpNode.mNumMeshes; i++)
		{
			processAnimatedMesh(asmpScene, asmpNode, *asmpScene.mMeshes[asmpNode.mMeshes[i]], model);
		}
	}

	// then go through each child in the node and process them as well
	if (asmpNode.mNumChildren > 0)
	{
		for (unsigned int x = 0; x < asmpNode.mNumChildren; x++)
		{
			processNode(asmpScene, *asmpNode.mChildren[x], model);
		}
	}

};
void _3DM::ModelLoader::processNode(const aiScene& asmpScene, const aiNode& asmpNode, Model& model)
{

#ifdef DEBUG
	printf("processing node ");
	printf(asmpNode.mName.C_Str());
	printf("\n");
#endif

	// cycle through each mesh within this node
	if (asmpNode.mNumMeshes > 0)
	{
		// cycle through each mesh
		for (unsigned int i = 0; i < asmpNode.mNumMeshes; i++)
		{
			processMesh(asmpScene, asmpNode, *asmpScene.mMeshes[asmpNode.mMeshes[i]], model);
		}
	}

	// then go through each child in the node and process them as well
	if (asmpNode.mNumChildren > 0)
	{
		for (unsigned int x = 0; x < asmpNode.mNumChildren; x++)
		{
			processNode(asmpScene, *asmpNode.mChildren[x], model);
		}
	}

};
void _3DM::ModelLoader::addNewTexture(_3DM::Mesh& mesh, const std::string & rootPath, const aiMaterial& mat, aiTextureType type)
{
	for (unsigned int i = 0; i < mat.GetTextureCount(type); i++)
	{
		ModelTexture newTexture;

		switch (type)
		{
		
		case aiTextureType_DIFFUSE:
			newTexture.imageType = TextureType::Diffuse;
			#ifdef DEBUG
			printf("Has diffuse textures : ");
			#endif
			break;
		
		case aiTextureType_SPECULAR:
			newTexture.imageType = TextureType::Specular;
			#ifdef DEBUG
			printf("Has specular textures : ");
			#endif
			break;

		case aiTextureType_NORMALS:
			newTexture.imageType = TextureType::Normals;
			#ifdef DEBUG
			printf("Has normals textures : ");
			#endif
			break;

		default:
			#ifdef DEBUG
			printf("Not a Valid Texture Type (ModelLoader.cpp)\n");
			#endif
			return;
		}


		#ifdef DEBUG
		printf("%i", mat.GetTextureCount(type));
		printf("\n");
		#endif

		aiString path;

		mat.GetTexture(type, i, &path);
				
		newTexture.imagePath = rootPath + path.C_Str();

		mesh.textures.push_back(newTexture);
	}
}


// add some error handling (not all models have uvs, etc)
void _3DM::ModelLoader::processAnimatedMesh(const aiScene& asmpScene, const aiNode& asmpNode, const aiMesh& asmpMesh, AnimatedModel& model)
{

#ifdef DEBUG
	printf("Processing a mesh: "); printf(asmpMesh.mName.C_Str());
	printf("\n");
	printf("Has bones: %i\n", asmpMesh.mNumBones);
#endif 

	AnimatedMesh tempMesh;

	tempMesh.weights.resize(asmpMesh.mNumVertices);

	for (unsigned int i = 0; i < tempMesh.weights.size(); i++)
	{
		for (int j = 0; j < tempMesh.weights.at(i).length(); j++)
		{
			tempMesh.weights.at(i)[j] = 0.0f;
		}
	}

	tempMesh.boneIDs.resize(asmpMesh.mNumVertices);
	for (unsigned int i = 0; i < tempMesh.boneIDs.size(); i++)
	{
		for (int j = 0; j < tempMesh.boneIDs.at(i).length(); j++)
		{
			tempMesh.boneIDs.at(i)[j] = -123;
		}
	}


	tempMesh.mesh.baseModelMatrix = toglmMat4(asmpNode.mTransformation);


	if (asmpNode.mParent)
	{
		tempMesh.mesh.baseModelMatrix = toglmMat4(asmpNode.mParent->mTransformation) * toglmMat4(asmpNode.mTransformation);
	}

	// cycle through each vertex in the mesh
	for (unsigned int i = 0; i < asmpMesh.mNumVertices; i++)
	{

		// load the vertices
		glm::vec3 tempV;
		glm::vec2 tempUV = glm::vec2(0, 0);

		tempV.x = asmpMesh.mVertices[i].x;
		tempV.y = asmpMesh.mVertices[i].y;
		tempV.z = asmpMesh.mVertices[i].z;
		tempMesh.mesh.vertices.push_back(tempV);

		if (asmpMesh.HasTextureCoords(0))
		{
			tempUV.x = asmpMesh.mTextureCoords[0][i].x;
			tempUV.y = asmpMesh.mTextureCoords[0][i].y;
		}

		tempMesh.mesh.uvs.push_back(tempUV);

		// load the normals (if they exist)
		if (asmpMesh.HasNormals())
		{
			glm::vec3 tempN;
			tempN.x = asmpMesh.mNormals[i].x;
			tempN.y = asmpMesh.mNormals[i].y;
			tempN.z = asmpMesh.mNormals[i].z;
			tempMesh.mesh.normals.push_back(tempN);
		}

	}

	// cycle through each face to get the indices
	for (unsigned int i = 0; i < asmpMesh.mNumFaces; i++)
	{

		for (unsigned int j = 0; j < asmpMesh.mFaces[i].mNumIndices; j++)
		{
			tempMesh.mesh.indices.push_back(asmpMesh.mFaces[i].mIndices[j]);
		}

	}

	if (asmpScene.HasMaterials())
	{
		// so that we don't have to type out that whole thing every time
		aiMaterial* mat = asmpScene.mMaterials[asmpMesh.mMaterialIndex];


		addNewTexture(tempMesh.mesh, model.rootPath, *mat, aiTextureType_DIFFUSE);
		addNewTexture(tempMesh.mesh, model.rootPath, *mat, aiTextureType_SPECULAR);
		addNewTexture(tempMesh.mesh, model.rootPath, *mat, aiTextureType_NORMALS);









		if (asmpMesh.HasBones())
		{

			for (unsigned int i = 0; i < asmpMesh.mNumBones; i++)
			{

				unsigned int boneID = 0;

				if (model.boneID.find(asmpMesh.mBones[i]->mName.data) == model.boneID.end())
				{
					boneID = model.boneID.size();
				}
				else
				{
					boneID = model.boneID[asmpMesh.mBones[i]->mName.data];
				}

				//Removed for now, May be needed later. As of now, I'm pretty sure I do not need it.
				//model.boneID[asmpMesh.mBones[i]->mName.data] = boneID;

				for (unsigned int j = 0; j < model.modelsAnimation.channels.size(); j++)
				{
					if (model.modelsAnimation.channels[j].name == asmpMesh.mBones[i]->mName.data)
					{
						model.modelsAnimation.boneOffset[asmpMesh.mBones[i]->mName.data] = toglmMat4(asmpMesh.mBones[i]->mOffsetMatrix);
					}
				}

				for (unsigned int j = 0; j < asmpMesh.mBones[i]->mNumWeights; j++)
				{
					unsigned int vertexID = asmpMesh.mBones[i]->mWeights[j].mVertexId;

					for (int k = 0; k < tempMesh.boneIDs.at(vertexID).length(); k++)
					{
						if (tempMesh.boneIDs.at(vertexID)[k] == -123)
						{
							tempMesh.boneIDs.at(vertexID)[k] = static_cast<float>(boneID);
							tempMesh.weights.at(vertexID)[k] = asmpMesh.mBones[i]->mWeights[j].mWeight;
							break;
						}
					}
				}
			}
		}

	}
	model.meshes.push_back(tempMesh);
}

glm::mat4 _3DM::toglmMat4(const aiMatrix4x4& ai)
{
	glm::mat4 mat;
	

	mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
	mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
	mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
	mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;

	return mat;
}

void _3DM::ModelLoader::buildBoneTree(const aiScene& asmpScene, const aiNode& asmpNode, BoneNode& parentBoneNode, const AnimatedModel& model)
{
	if (asmpScene.HasAnimations())
	{

		bool isBoneNode = false;

#ifdef DEBUG
		printf("Processing Node ");
		printf(asmpNode.mName.data);
		printf("\n");
#endif
		/**************************************************************************************************
		* if the current ainode's name is the same as one of the bone id's names, then it is a bone node. *
		***************************************************************************************************/

		if (model.boneID.find(asmpNode.mName.data) != model.boneID.end())
		{

#ifdef DEBUG
			printf("Node : ");
			printf(asmpNode.mName.data);
			printf(" is a bone node.\n\n");
#endif

			isBoneNode = true;

			//create a 3DM bone node
			BoneNode tempNode;

			//set it's name to be the same as the aiNode's name.
			tempNode.name = asmpNode.mName.data;

			//add this bone as a child to the current parent bone node.
			parentBoneNode.children.push_back(tempNode);

		}
#ifdef DEBUG
		else
		{
			printf("Node : ");
			printf(asmpNode.mName.data);
			printf(" is not a bone node.\n");
		}
#endif

		//if the current aiNode has children
		if (asmpNode.mNumChildren > 0)
		{

			// if the node we just found was a bone node then pass it in as the new parent bone node. (current bone node child vector size - 1 is the new bone node's location) 
			if (isBoneNode)
			{
				//loop through all of the aiNode's children
				for (unsigned int i = 0; i < asmpNode.mNumChildren; i++)
				{
					buildBoneTree(asmpScene, *asmpNode.mChildren[i], parentBoneNode.children[parentBoneNode.children.size() - 1], model);
				}
			}
			else //if the aiNode is npot a bone node, then we pass in the current parent BoneNode Again
			{
				//loop through all of the aiNode's children
				for (unsigned int i = 0; i < asmpNode.mNumChildren; i++)
				{
					buildBoneTree(asmpScene, *asmpNode.mChildren[i], parentBoneNode, model);
				}
			}

		}

	}

}