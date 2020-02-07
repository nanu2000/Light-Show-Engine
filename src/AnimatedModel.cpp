#include "AnimatedModel.h"

void _3DM::AnimatedModel::removeKeyframes(unsigned int channelIndex) {
    if (channelIndex < modelsAnimation.channels.size()) {
        modelsAnimation.channels.erase(modelsAnimation.channels.begin() + channelIndex);
    }
}

void _3DM::AnimatedModel::addScaleToKeyFrames(const glm::vec3& scale, unsigned int channelIndex) {
    if (channelIndex < modelsAnimation.channels.size()) {
        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).scalingKeys.size(); i++) {
            modelsAnimation.channels.at(channelIndex).scalingKeys[i].origin += scale;
        }
    }
#ifdef DEBUG
    else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
#endif
}

void _3DM::AnimatedModel::overwriteScaleKeyFrames(const glm::vec3& scale, unsigned int channelIndex) {

    if (channelIndex < modelsAnimation.channels.size()) {
        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).scalingKeys.size(); i++) {
            modelsAnimation.channels.at(channelIndex).scalingKeys[i].origin = scale;
        }
    }
#ifdef DEBUG
    else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
#endif
}

void _3DM::AnimatedModel::addPositionToKeyFrames(const glm::vec3& position, unsigned int channelIndex) {

    if (channelIndex < modelsAnimation.channels.size()) {
        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).positionKeys.size(); i++) {
            modelsAnimation.channels.at(channelIndex).positionKeys[i].origin += position;
        }
    } else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
}

void _3DM::AnimatedModel::overWritePositionToKeyFrames(const glm::vec3& position, unsigned int channelIndex) {
    if (channelIndex < modelsAnimation.channels.size()) {
        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).positionKeys.size(); i++) {
            modelsAnimation.channels.at(channelIndex).positionKeys[i].origin = position;
        }
    } else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
}

void _3DM::AnimatedModel::initialize(Shader& shader) {
    // loop through each mesh and initialize them
    for (unsigned int i = 0; i < meshes.size(); i++) {

        initializeTexture(meshes[i].mesh, shader);
        initializeBuffers(meshes[i].mesh, shader);
        initializeBoneBuffers(meshes[i], shader);

        glBindVertexArray(0);
    }
    initialized   = true;
    animatedModel = true;
}

_3DM::AnimatedModel::~AnimatedModel() {

    if (!initialized) {
        //If the object is not initialized then no buffers were generated.
        return;
    }

    DBG_LOG("Freeing memory for animated model.\n");

    for (unsigned int i = 0; i < meshes.size(); i++) {
        AnimatedMesh& mesh = meshes.at(i);

        glDeleteBuffers(1, &mesh.weightsBufferObject);
        glDeleteBuffers(1, &mesh.boneIDsBufferObject);
        glDeleteVertexArrays(1, &mesh.mesh.vertexArrayObject);
        glDeleteBuffers(1, &mesh.mesh.vertexBufferObject);
        glDeleteBuffers(1, &mesh.mesh.elementBufferObject);
        glDeleteBuffers(1, &mesh.mesh.uvBufferObject);
        glDeleteBuffers(1, &mesh.mesh.normalBufferObject);
    }
}

_3DM::AnimatedModel::AnimatedModel(const std::string& path) {
    _3DM_IO modelLoader;
    *this = modelLoader.readAnimatedModel(path);

    if (!modelLoaded) {
        printf("Please load in a model before initializing buffers. ( _3DM::Model::initialize() )\n");
        return;
    }
}

glm::mat4 _3DM::AnimatedModel::getMeshMatrix(unsigned int index) const {
    if (index < meshes.size() && index >= 0) {
        return meshes.at(index).mesh.baseModelMatrix;
    } else {
        DBG_LOG("Index went out of bounds (glm::mat4 _3DM::AnimatedModel::getMeshMatrix in AnimatedModel.cpp)\n");
    }

    return glm::mat4();
}

void _3DM::AnimatedModel::setMeshMatrix(unsigned int index, const glm::mat4& newMatrix) {
    if (index < meshes.size() && index >= 0) {
        meshes.at(index).mesh.baseModelMatrix = newMatrix;
    }
}

void _3DM::AnimatedModel::addRotationToKeyFrames(const glm::quat& rotation, unsigned int channelIndex) {
    if (channelIndex < modelsAnimation.channels.size()) {
        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).rotationKeys.size(); i++) {
            modelsAnimation.channels.at(channelIndex).rotationKeys[i].origin += rotation;
        }
    } else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
}

void _3DM::AnimatedModel::addRotationToKeyFrames(const glm::vec3& rotation, unsigned int channelIndex) {

    if (channelIndex < modelsAnimation.channels.size()) {

        glm::vec3 rot;
        rot.x = glm::radians(rotation.x);
        rot.y = glm::radians(rotation.y);
        rot.z = glm::radians(rotation.z);

        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).rotationKeys.size(); i++) {

            modelsAnimation.channels.at(channelIndex).rotationKeys[i].origin.w = 1;
            modelsAnimation.channels.at(channelIndex).rotationKeys[i].origin.x += rot.x;
            modelsAnimation.channels.at(channelIndex).rotationKeys[i].origin.y += rot.y;
            modelsAnimation.channels.at(channelIndex).rotationKeys[i].origin.z += rot.z;
        }
    } else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
}

void _3DM::AnimatedModel::overwriteRotationKeyFrames(const glm::vec3& rotation, unsigned int channelIndex) {

    if (channelIndex < modelsAnimation.channels.size()) {

        glm::quat rot = hh::toQuaternion(rotation);

        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).rotationKeys.size(); i++) {
            modelsAnimation.channels.at(channelIndex).rotationKeys[i].origin = rot;
        }
    } else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
}

void _3DM::AnimatedModel::overwriteRotationKeyFrames(const glm::quat& rotation, unsigned int channelIndex) {
    if (channelIndex < modelsAnimation.channels.size()) {
        for (unsigned int i = 0; i < modelsAnimation.channels.at(channelIndex).rotationKeys.size(); i++) {
            modelsAnimation.channels.at(channelIndex).rotationKeys[i].origin = rotation;
        }
    } else {
        DBG_LOG("local variable channelIndex goes out of bounds.\n");
    }
}

glm::mat4 _3DM::AnimatedModel::getBoneTransformation(unsigned int boneId) const {
    if (boneId < modelsAnimation.boneTransformations.size()) {
        return modelsAnimation.boneTransformations.at(boneId);
    } else {
        DBG_LOG("local variable boneId goes out of bounds.\n");
        return glm::mat4(1.0);
    }
}

glm::mat4 _3DM::AnimatedModel::getBoneTransformationWithoutOffset(unsigned int boneId) const {

    std::map<std::string, glm::mat4>::const_iterator boneMatrixIT = modelsAnimation.boneOffset.find(getBoneName(boneId));

    if (boneMatrixIT != modelsAnimation.boneOffset.end()) {
        return getBoneTransformation(boneId) / boneMatrixIT->second; //Use matrix 'division' to undo the multiplication of the bone offset.
    }
    return glm::mat4();
}

glm::mat4 _3DM::AnimatedModel::getBoneTransformationWithoutOffset(const std::string& name) const {
    std::map<std::string, uint32_t>::const_iterator boneIDNameIT = boneIDMap.find(name);

    if (boneIDNameIT != boneIDMap.end()) {

        std::map<std::string, glm::mat4>::const_iterator boneMatrixIT = modelsAnimation.boneOffset.find(boneIDNameIT->first);

        if (boneMatrixIT != modelsAnimation.boneOffset.end()) {
            return getBoneTransformation(getBoneID(name)) / boneMatrixIT->second; //Use matrix 'division' to undo the multiplication of the bone offset.
        }
    }

    return glm::mat4();
}

std::vector<glm::vec3>* _3DM::AnimatedModel::getMeshVertices(unsigned int index) {
    if (index < meshes.size()) {
        return &(meshes.at(index).mesh.vertices);
    }
    return nullptr;
}

std::vector<uint32_t>* _3DM::AnimatedModel::getMeshIndices(unsigned int index) {
    if (index < meshes.size()) {
        return &(meshes.at(index).mesh.indices);
    }
    return nullptr;
}

void _3DM::AnimatedModel::setBoneMatrix(const glm::mat4& transformation, unsigned int boneId) {
    if (boneId < modelsAnimation.boneTransformations.size()) {
        modelsAnimation.boneTransformations.at(boneId) = transformation;
    } else {
        DBG_LOG("There was an error setting the bone matrix. local variable boneId goes out of bounds.\n");
    }
}
/*********************************************************************************************************

*Will update the animation accordingly. the value 'time' is expected to be a constantly increasing value.*
**********************************************************************************************************/
void _3DM::AnimatedModel::fixedUpdateAnimation() {
    if (currentAnimationClip != lastAnimationClip) {
        blendingLastFrameTime     = timeSinceAnimationStarted;
        blendinglastAnimationClip = lastAnimationClip;

        currentBlendingTime       = 0;
        timeSinceAnimationStarted = 0;
        lastAnimationClip         = currentAnimationClip;
    }

    if (currentBlendingTime < blendingTime) {
        blendBoneTree(
            blendingLastFrameTime * modelsAnimation.ticksPerSecond,
            &modelsAnimation.rootBone, //the root bone node
            glm::mat4(1.0f) //an Identity matrix, since the root bone node has no parents.
        );
        currentBlendingTime += GameInfo::fixedDeltaTime;
    } else {
        //if this doesn't work, then maybe you did not initialize the model, or its not the right format.
        updateBoneTree(
            timeSinceAnimationStarted * modelsAnimation.ticksPerSecond, //current time multiplied by the current frames ticks per second.
            &modelsAnimation.rootBone, //the root bone node
            glm::mat4(1.0f) //an Identity matrix, since the root bone node has no parents.
        );

        timeSinceAnimationStarted += GameInfo::fixedDeltaTime;
    }
}

//Returns -1 on failure.
int _3DM::AnimatedModel::getMeshIndex(const std::string& MeshName) const {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        if (meshes[i].mesh.name == MeshName) {
            return i;
        }
    }
    return -1;
}

void _3DM::AnimatedModel::renderSingleMesh(unsigned int index, Shader& shader) {
    if (!modelLoaded) {
        DBG_LOG("Please load a model before trying to render one.\n");
        return;
    }
    if (index < meshes.size()) {
        renderMesh(index, shader);
    } else {
        DBG_LOG("This index goes out of bounds (_3DM::Model::renderSingleMesh)\n");
    }
}

void _3DM::AnimatedModel::setAnimationClip(unsigned int clip) {
    if (clip < animationClips.size()) {
        currentAnimationClip = clip;
    }
#ifdef DEBUG
    else {
        DBG_LOG("Animation clip index goes out of bounds! **URGENT**\n");
    }
#endif
}

//Returns -1 on failure of finding animation clip
int _3DM::AnimatedModel::getAnimationIndex(const std::string& name) const {
    for (unsigned int i = 0; i < animationClips.size(); i++) {
        if (name == animationClips[i].name) {
            return i;
        }
    }
    return -1;
}

//Returns -1 on failure of finding bone ID
int _3DM::AnimatedModel::getBoneID(const std::string& nodeName) const {
    std::map<std::string, uint32_t>::const_iterator boneIDNameIT = boneIDMap.find(nodeName);

    if (boneIDNameIT != boneIDMap.end()) {
        return boneIDNameIT->second;
    }
    return -1;
}

std::string _3DM::AnimatedModel::getBoneName(uint32_t id) const {
    for (std::map<std::string, uint32_t>::const_iterator it = boneIDMap.begin(); it != boneIDMap.end(); it++) {
        if (id == it->second) {
            return it->first;
        }
    }
    DBG_LOG("Bone ID Does not reveal any assosiated strings in map. (std::string _3DM::AnimatedModel::getBoneName)\n");
    return "";
}

//Returns -1 on failure of finding channel's index
int _3DM::AnimatedModel::getChannelIndex(const std::string& channelName) const {

    for (unsigned int i = 0; i < modelsAnimation.channels.size(); i++) {
        if (channelName == modelsAnimation.channels[i].name) {
            return i;
        }
    }
    return -1;
}

/*****************************************************************************************************************************************
*This function will recursively update the bonetree according to the time given. it will also interpolate properly between each keyframe.*
******************************************************************************************************************************************/
void _3DM::AnimatedModel::updateBoneTree(const float& timeInTicks, _3DM::BoneNode* node, const glm::mat4& parentTransform) {

    int channelIndex = getChannelIndex(node->name);

    //this gets the current time of the animation with the given time variable (timeInTicks).
    glm::mat4 finalModel = glm::mat4(1.0f);

    /*****************************************************************************************************************
	*Interpolate the bone nodes translation, rotation, and scale, only if it has 2 or more keyframes in it's channel.*
	*Here we use the Helping hand Lerp function, and the glm::slerp function for interpolation.						 *
	******************************************************************************************************************/
    if (node->name != "rootBoneTreeNode" && channelIndex != -1) {

        float currentTime = std::fmod //get floating point modulus.
            (
                timeInTicks,
                animationClips.at(currentAnimationClip).endTime - animationClips.at(currentAnimationClip).startTime);

        if (std::isnan(currentTime)) {
            currentTime = 0;
        }

        currentTime += animationClips.at(currentAnimationClip).startTime;

        //Get the current keyframes in the animation, and the interpolation value (0.0-1.0)
        InterpolatedFrame translationKeysIndex = getKeyframesAtTime(modelsAnimation.channels.at(channelIndex).positionKeys, currentTime);
        InterpolatedFrame scaleKeysIndex       = getKeyframesAtTime(modelsAnimation.channels.at(channelIndex).scalingKeys, currentTime);
        InterpolatedFrame rotationKeysIndex    = getKeyframesAtTime(modelsAnimation.channels.at(channelIndex).rotationKeys, currentTime);

        /********************************************************************************************************************
		*Here, we create the transformation matrix for this bone node.														*
		*We multiply the parent bone node's transformation matrix by this nodes translation, rotation and scaling matricies.*
		*********************************************************************************************************************/

        finalModel = parentTransform
            * glm::translate(
                         glm::mat4(1.0f),

                         hh::lerp //translation
                         (
                             modelsAnimation.channels.at(channelIndex).positionKeys.at(translationKeysIndex.firstFrame).origin,
                             modelsAnimation.channels.at(channelIndex).positionKeys.at(translationKeysIndex.lastFrame).origin,
                             translationKeysIndex.interpolation))
            * glm::mat4_cast(
                         glm::slerp //rotation
                         (
                             modelsAnimation.channels.at(channelIndex).rotationKeys.at(rotationKeysIndex.firstFrame).origin,
                             modelsAnimation.channels.at(channelIndex).rotationKeys.at(rotationKeysIndex.lastFrame).origin,
                             rotationKeysIndex.interpolation))
            * glm::scale(
                         glm::mat4(1.0f),
                         hh::lerp //scale
                         (
                             modelsAnimation.channels.at(channelIndex).scalingKeys.at(scaleKeysIndex.firstFrame).origin,
                             modelsAnimation.channels.at(channelIndex).scalingKeys.at(scaleKeysIndex.lastFrame).origin,
                             scaleKeysIndex.interpolation));

        int boneIndex = getBoneID(node->name);

        if (boneIndex != -1) {
            modelsAnimation.boneTransformations.at(boneIndex) = finalModel * modelsAnimation.boneOffset[node->name];

            //DBG_LOG(" %s Offset = %f, %f, %f\n\n", node->name.c_str(), modelsAnimation.boneOffset[node->name][3][0], modelsAnimation.boneOffset[node->name][3][1], modelsAnimation.boneOffset[node->name][3][2]);

        } else {
            modelsAnimation.boneTransformations.at(boneIDMap[node->name]) = finalModel;
        }
    }

    /******************************************************************************************************
	*Here, we loop through this bone nodes children, and recursivly update their transformation matricies.*
	*******************************************************************************************************/
    for (unsigned int i = 0; i < node->children.size(); i++) {
        updateBoneTree(timeInTicks, &node->children[i], finalModel);
    }
}
/*****************************************************************************************************************************************
*This function will recursively blend the bonetree according to the time given. it will also interpolate properly between each keyframe.*
******************************************************************************************************************************************/
void _3DM::AnimatedModel::blendBoneTree(const float& lastAnimationTime, _3DM::BoneNode* node, const glm::mat4& parentTransform) {

    int channelIndex = getChannelIndex(node->name);

    //this gets the current time of the animation with the given time variable (timeInTicks).
    glm::mat4 finalModel = glm::mat4(1.0f);

    /*****************************************************************************************************************
	*Interpolate the bone nodes translation, rotation, and scale, only if it has 2 or more keyframes in it's channel.*
	*Here we use the Helping hand Lerp function, and the glm::slerp function for interpolation.						 *
	******************************************************************************************************************/
    if (node->name != "rootBoneTreeNode" && channelIndex != -1) {

        float currentTime = std::fmod //get floating point modulus.
            (
                lastAnimationTime,
                animationClips.at(blendinglastAnimationClip).endTime - animationClips.at(blendinglastAnimationClip).startTime);

        if (std::isnan(currentTime)) {
            currentTime = 0;
        }

        currentTime += animationClips.at(blendinglastAnimationClip).startTime;

        //Get the current keyframes in the animation, and the interpolation value (0.0-1.0)
        InterpolatedFrame translationKeysIndex = getKeyframesAtTime(modelsAnimation.channels.at(channelIndex).positionKeys, currentTime);
        InterpolatedFrame scaleKeysIndex       = getKeyframesAtTime(modelsAnimation.channels.at(channelIndex).scalingKeys, currentTime);
        InterpolatedFrame rotationKeysIndex    = getKeyframesAtTime(modelsAnimation.channels.at(channelIndex).rotationKeys, currentTime);

        //For fast blending, set orientation to the first index (???KeysIndex.x) and do not lerp / slerp.

        glm::vec3 aiTranslation = hh::lerp //translation
            (
                modelsAnimation.channels.at(channelIndex).positionKeys.at(translationKeysIndex.firstFrame).origin,
                modelsAnimation.channels.at(channelIndex).positionKeys.at(translationKeysIndex.lastFrame).origin,
                translationKeysIndex.interpolation);

        glm::vec3 aiScale = hh::lerp //scale
            (
                modelsAnimation.channels.at(channelIndex).scalingKeys.at(scaleKeysIndex.firstFrame).origin,
                modelsAnimation.channels.at(channelIndex).scalingKeys.at(scaleKeysIndex.lastFrame).origin,
                scaleKeysIndex.interpolation);

        glm::quat aiRotation = glm::slerp //rotation
            (
                modelsAnimation.channels.at(channelIndex).rotationKeys.at(rotationKeysIndex.firstFrame).origin,
                modelsAnimation.channels.at(channelIndex).rotationKeys.at(rotationKeysIndex.lastFrame).origin,
                rotationKeysIndex.interpolation);

        float interpolationValue = currentBlendingTime / blendingTime;

        aiTranslation = hh::lerp //translation
            (
                aiTranslation,
                modelsAnimation.channels.at(channelIndex).positionKeys.at(getNearestFrameAtTime(modelsAnimation.channels.at(channelIndex).positionKeys, animationClips.at(currentAnimationClip).startTime)).origin,
                interpolationValue);

        aiScale = hh::lerp //scale
            (
                aiScale,
                modelsAnimation.channels.at(channelIndex).scalingKeys.at(getNearestFrameAtTime(modelsAnimation.channels.at(channelIndex).scalingKeys, animationClips.at(currentAnimationClip).startTime)).origin,
                interpolationValue);

        aiRotation = glm::slerp //rotation
            (
                aiRotation,
                modelsAnimation.channels.at(channelIndex).rotationKeys.at(getNearestFrameAtTime(modelsAnimation.channels.at(channelIndex).rotationKeys, animationClips.at(currentAnimationClip).startTime)).origin,
                interpolationValue);

        /********************************************************************************************************************
		*Here, we create the transformation matrix for this bone node.														*
		*We multiply the parent bone node's transformation matrix by this nodes translation, rotation and scaling matricies.*
		*********************************************************************************************************************/

        finalModel = parentTransform
            * glm::translate(glm::mat4(1.0f), aiTranslation) //create this nodes translation matrix.
            * glm::mat4_cast(aiRotation) //create this nodes rotation matrix.
            * glm::scale(glm::mat4(1.0f), aiScale); //create this nodes scaling matrix.

        int boneIndex = getBoneID(node->name);

        if (boneIndex != -1) {
            modelsAnimation.boneTransformations.at(boneIndex) = finalModel * modelsAnimation.boneOffset[node->name];
        } else {
            modelsAnimation.boneTransformations.at(boneIDMap[node->name]) = finalModel;
        }
    }

    /******************************************************************************************************
	*Here, we loop through this bone nodes children, and recursivly update their transformation matricies.*
	*******************************************************************************************************/
    for (unsigned int i = 0; i < node->children.size(); i++) {
        blendBoneTree(lastAnimationTime, &node->children[i], finalModel);
    }
}

/*************************************************************
* This function is expected to be called after glUseProgram. *
* This function contains NO bounds checking					 *
**************************************************************/
void _3DM::AnimatedModel::renderMesh(unsigned int index, Shader& shader) {
    glm::mat4 transformation = meshes.at(index).mesh.baseModelMatrix;

    transformation = glm::translate(transformation, transform.position);
    transformation = glm::rotate(transformation, glm::angle(transform.rotation), glm::axis(transform.rotation));
    transformation = glm::scale(transformation, transform.scale);

    glBindVertexArray(meshes.at(index).mesh.vertexArrayObject); //Bind VAO

    glUniformMatrix4fv(
        Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ModelMatrix),
        1,
        GL_FALSE,
        glm::value_ptr(transformation));

    glUniformMatrix4fv //supply bone matricies for shader
        (
            Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::BoneTransformations),
            modelsAnimation.boneTransformations.size(),
            GL_FALSE,
            glm::value_ptr(modelsAnimation.boneTransformations[0]));

    for (GLuint j = 0; j < meshes.at(index).mesh.textures.size(); j++) {

        glActiveTexture(GL_TEXTURE0 + j); // Activate texture before binding

        glUniform1i(
            glGetUniformLocation(
                shader.getProgramID(),
                meshes.at(index).mesh.textures.at(j).uniformName.c_str()),
            j);

        glBindTexture(GL_TEXTURE_2D, meshes.at(index).mesh.textures.at(j).imageID);
    }

    glDrawElements(GL_TRIANGLES, meshes.at(index).mesh.indices.size(), GL_UNSIGNED_INT, 0); //Draw the mesh

    glBindVertexArray(0);
}
/****************************************
*Render All meshes if they exist.		*
*****************************************/
void _3DM::AnimatedModel::renderAll(Shader& shader) {

    if (!modelLoaded) {
        DBG_LOG("Please load a model before trying to render one.\n");
        return;
    }

    //Loop through all of the meshes in the model
    for (unsigned int i = 0; i < meshes.size(); i++) {
        renderMesh(i, shader);
    }
};

void _3DM::AnimatedModel::initializeBoneBuffers(_3DM::AnimatedMesh& mesh, Shader& shader) {
    glGenBuffers(1, &mesh.weightsBufferObject);
    glGenBuffers(1, &mesh.boneIDsBufferObject);

    bindBuffers(shader, mesh);
}

void _3DM::AnimatedModel::bindBuffers(Shader& shader, AnimatedMesh& mesh) {
    int boneIDsAttribute = Shaders::getAttribLocation(Shaders::AttribName::BoneIDS);
    int weightsAttribute = Shaders::getAttribLocation(Shaders::AttribName::BoneWeights);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.weightsBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mesh.weights.size(), &mesh.weights[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(weightsAttribute);
    glVertexAttribPointer(weightsAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.boneIDsBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mesh.boneIDs.size(), &mesh.boneIDs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(boneIDsAttribute);
    glVertexAttribPointer(boneIDsAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void _3DM::AnimatedModel::initializeTexture(_3DM::Mesh& mesh, Shader& shader) {

    for (GLuint j = 0; j < mesh.textures.size(); j++) {

        //Check to see if uniform is already set. If it is then the texture was already initialized (most likely via addTexture).
        if (!mesh.textures.at(j).uniformName.empty()) {
            continue;
        }

        std::stringstream stringStream;

        mesh.textures.at(j).imageID = TextureLocator::getService().getTexture(mesh.textures.at(j).imagePath).getTextureData();

        if (mesh.textures.at(j).imageType == TextureType::Diffuse) {
            mesh.diffuseIndex++;
            stringStream << mesh.diffuseIndex;

            mesh.textures.at(j).uniformName = Shaders::getUniformName(Shaders::UniformName::DiffuseTexture) + stringStream.str();

            continue;
        }

        if (mesh.textures.at(j).imageType == TextureType::Normals) {
            mesh.normalsIndex++;
            stringStream << mesh.normalsIndex;

            mesh.textures.at(j).uniformName = Shaders::getUniformName(Shaders::UniformName::NormalTexture) + stringStream.str();
            continue;
        }

        if (mesh.textures.at(j).imageType == TextureType::Specular) {

            mesh.specularIndex++;
            stringStream << mesh.specularIndex;

            mesh.textures.at(j).uniformName = Shaders::getUniformName(Shaders::UniformName::SpecularTexture) + stringStream.str();
            continue;
        }
    }
}
void _3DM::AnimatedModel::initializeBuffers(_3DM::Mesh& mesh, Shader& shader) {
    glGenVertexArrays(1, &mesh.vertexArrayObject);

    glGenBuffers(1, &mesh.vertexBufferObject);
    glGenBuffers(1, &mesh.elementBufferObject);
    glGenBuffers(1, &mesh.uvBufferObject);
    glGenBuffers(1, &mesh.normalBufferObject);

    GLint positionAttribute = Shaders::getAttribLocation(Shaders::AttribName::Position);
    GLint uvAttribute       = Shaders::getAttribLocation(Shaders::AttribName::TextureCoordinates);
    GLint normalsAttribute  = Shaders::getAttribLocation(Shaders::AttribName::Normal);

    glBindVertexArray(mesh.vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvs.size(), &mesh.uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(uvAttribute);
    glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), &mesh.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(normalsAttribute);
    glVertexAttribPointer(normalsAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
