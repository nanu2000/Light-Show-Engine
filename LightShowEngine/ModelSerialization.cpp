#include "ModelSerialization.h"

void _3DM::_3DM_IO::writeBoneHierarchyToFile(_3DM::BoneNode& boneNode, std::ofstream& oStream) {
    Byte sizeOfName;
    TwoBytes amountOfChildren;

    amountOfChildren = static_cast<TwoBytes>(boneNode.children.size());
    sizeOfName       = static_cast<Byte>(boneNode.name.size());

    writeBytes(sizeOfName, 1, oStream);

    writeBytes(boneNode.name.c_str(), sizeOfName, oStream);

    writeBytes(amountOfChildren, 2, oStream);

    for (unsigned int i = 0; i < boneNode.children.size(); i++) {
        writeBoneHierarchyToFile(boneNode.children[i], oStream);
    }
}

void _3DM::_3DM_IO::readBoneHierarchyFromFile(std::ifstream& iStream, _3DM::BoneNode& futureRootNode) {

    Byte sizeOfName = 0;
    TwoBytes amountOfChildren;
    const char* newStr = nullptr;

    //since sizzeofname is one byte, we dont have to worry about endianness.

    readBytes(sizeOfName, 1, iStream, isBigEndianness);

    if (sizeOfName > 0) {
        newStr = new char[sizeOfName];

        readBytes(newStr, sizeOfName, iStream);

        futureRootNode.name = std::string(newStr, sizeOfName);

        delete[] newStr;
        newStr = nullptr;
    }

    readBytes(amountOfChildren, 2, iStream, isBigEndianness);

    if (amountOfChildren > 0) {
        futureRootNode.children.resize(amountOfChildren);

        for (unsigned int i = 0; i < amountOfChildren; i++) {
            readBoneHierarchyFromFile(iStream, futureRootNode.children[i]);
        }
    }
}

void _3DM::_3DM_IO::writeChannel(_3DM::Channel& channel, std::ofstream& oStream) {

    Byte sizeOfName;
    TwoBytes sizeOfPositionKeys = 0;
    TwoBytes sizeOfScalingKeys  = 0;
    TwoBytes sizeOfRotationKeys = 0;

    sizeOfName = static_cast<Byte>(channel.name.size());

    sizeOfPositionKeys = static_cast<TwoBytes>(channel.positionKeys.size());
    sizeOfRotationKeys = static_cast<TwoBytes>(channel.rotationKeys.size());
    sizeOfScalingKeys  = static_cast<TwoBytes>(channel.scalingKeys.size());

    oStream.write(reinterpret_cast<char*>(&sizeOfName), 1);

    oStream.write(const_cast<char*>(channel.name.c_str()), sizeOfName);

    oStream.write(reinterpret_cast<char*>(&sizeOfPositionKeys), 2);

    for (unsigned int i = 0; i < sizeOfPositionKeys; i++) {

        //Time
        oStream.write(reinterpret_cast<char*>(&channel.positionKeys[i].time), SIZE_OF_FLOAT);

        //Origin.x
        oStream.write(reinterpret_cast<char*>(&channel.positionKeys[i].origin.x), SIZE_OF_FLOAT);

        //Origin.y
        oStream.write(reinterpret_cast<char*>(&channel.positionKeys[i].origin.y), SIZE_OF_FLOAT);

        //Origin.z
        oStream.write(reinterpret_cast<char*>(&channel.positionKeys[i].origin.z), SIZE_OF_FLOAT);
    }

    oStream.write(reinterpret_cast<char*>(&sizeOfScalingKeys), 2);

    for (unsigned int i = 0; i < sizeOfScalingKeys; i++) {

        //Time
        oStream.write(reinterpret_cast<char*>(&channel.scalingKeys[i].time), SIZE_OF_FLOAT);

        //Origin.x
        oStream.write(reinterpret_cast<char*>(&channel.scalingKeys[i].origin.x), SIZE_OF_FLOAT);

        //Origin.y
        oStream.write(reinterpret_cast<char*>(&channel.scalingKeys[i].origin.y), SIZE_OF_FLOAT);

        //Origin.z
        oStream.write(reinterpret_cast<char*>(&channel.scalingKeys[i].origin.z), SIZE_OF_FLOAT);
    }

    oStream.write(reinterpret_cast<char*>(&sizeOfRotationKeys), 2);
    for (unsigned int i = 0; i < sizeOfRotationKeys; i++) {

        //Time
        oStream.write(reinterpret_cast<char*>(&channel.rotationKeys.at(i).time), SIZE_OF_FLOAT);

        //Origin.w
        oStream.write(reinterpret_cast<char*>(&channel.rotationKeys[i].origin.w), SIZE_OF_FLOAT);

        //Origin.x
        oStream.write(reinterpret_cast<char*>(&channel.rotationKeys[i].origin.x), SIZE_OF_FLOAT);

        //Origin.y
        oStream.write(reinterpret_cast<char*>(&channel.rotationKeys[i].origin.y), SIZE_OF_FLOAT);

        //Origin.z
        oStream.write(reinterpret_cast<char*>(&channel.rotationKeys[i].origin.z), SIZE_OF_FLOAT);
    }
}

_3DM::Channel _3DM::_3DM_IO::readChannel(std::ifstream& iStream) {

    _3DM::Channel newChannel;

    Byte sizeOfName;
    TwoBytes sizeOfPositionKeys = 0;
    TwoBytes sizeOfScalingKeys  = 0;
    TwoBytes sizeOfRotationKeys = 0;
    const char* newStr          = nullptr;

    readBytes(sizeOfName, 1, iStream, isBigEndianness);

    if (sizeOfName > 0) {

        newStr = new char[sizeOfName];

        readBytes(newStr, sizeOfName, iStream);

        newChannel.name = std::string(newStr, sizeOfName);

        delete[] newStr;

        newStr = nullptr;
    }

    readBytes(sizeOfPositionKeys, 2, iStream, isBigEndianness);

    if (sizeOfPositionKeys > 0) {
        newChannel.positionKeys.resize(sizeOfPositionKeys);

        for (unsigned int i = 0; i < sizeOfPositionKeys; i++) {

            readBytes(newChannel.positionKeys[i].time, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.positionKeys[i].origin.x, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.positionKeys[i].origin.y, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.positionKeys[i].origin.z, SIZE_OF_FLOAT, iStream, isBigEndianness);
        }
    }

    readBytes(sizeOfScalingKeys, 2, iStream, isBigEndianness);

    if (sizeOfScalingKeys > 0) {
        newChannel.scalingKeys.resize(sizeOfScalingKeys);

        for (unsigned int i = 0; i < sizeOfScalingKeys; i++) {

            readBytes(newChannel.scalingKeys[i].time, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.scalingKeys[i].origin.x, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.scalingKeys[i].origin.y, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.scalingKeys[i].origin.z, SIZE_OF_FLOAT, iStream, isBigEndianness);
        }
    }

    readBytes(sizeOfRotationKeys, 2, iStream, isBigEndianness);

    if (sizeOfRotationKeys > 0) {
        newChannel.rotationKeys.resize(sizeOfRotationKeys);

        for (unsigned int i = 0; i < sizeOfRotationKeys; i++) {
            readBytes(newChannel.rotationKeys[i].time, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.rotationKeys[i].origin.w, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.rotationKeys[i].origin.x, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.rotationKeys[i].origin.y, SIZE_OF_FLOAT, iStream, isBigEndianness);
            readBytes(newChannel.rotationKeys[i].origin.z, SIZE_OF_FLOAT, iStream, isBigEndianness);
        }
    }

    return newChannel;
}

void _3DM::_3DM_IO::writeAnimation(_3DM::Animation& animation, std::ofstream& oStream) {

    Byte sizeOfBoneOffsets    = static_cast<Byte>(animation.boneOffset.size());
    Byte sizeOfBoneTransforms = static_cast<Byte>(animation.boneTransformations.size());

    writeBytes(animation.duration, SIZE_OF_FLOAT, oStream);
    writeBytes(animation.ticksPerSecond, SIZE_OF_FLOAT, oStream);

    writeBoneHierarchyToFile(animation.rootBone, oStream);
    writeVector<_3DM::Channel>([&](_3DM::Channel& channel, std::ofstream& oS) { writeChannel(channel, oS); }, animation.channels, oStream);

    writeBytes(sizeOfBoneOffsets, 1, oStream);

    for (std::map<std::string, glm::mat4>::iterator i = animation.boneOffset.begin(); i != animation.boneOffset.end(); ++i) {
        Byte boneOffsetSizeOfName = static_cast<Byte>(i->first.size());

        writeBytes(boneOffsetSizeOfName, 1, oStream);
        writeBytes(i->first.c_str(), boneOffsetSizeOfName, oStream);

        for (int j = 0; j < Mat_4_ROW_COL_SIZE; j++) {
            for (int k = 0; k < Mat_4_ROW_COL_SIZE; k++) {
                writeBytes(i->second[j][k], SIZE_OF_FLOAT, oStream);
            }
        }
    }

    writeBytes(sizeOfBoneTransforms, 1, oStream);

    for (unsigned int i = 0; i < animation.boneTransformations.size(); i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                writeBytes(animation.boneTransformations[i][j][k], SIZE_OF_FLOAT, oStream);
            }
        }
    }
}

_3DM::Animation _3DM::_3DM_IO::readAnimation(std::ifstream& iStream) {
    _3DM::Animation outputAnimation;

    Byte sizeOfBoneOffsets = 0;
    const char* newStr     = nullptr;

    readBytes(outputAnimation.duration, SIZE_OF_FLOAT, iStream, isBigEndianness);
    readBytes(outputAnimation.ticksPerSecond, SIZE_OF_FLOAT, iStream, isBigEndianness);

    readBoneHierarchyFromFile(iStream, outputAnimation.rootBone);

    outputAnimation.channels = readVector<_3DM::Channel>([&]() -> _3DM::Channel { return readChannel(iStream); }, iStream);

    readBytes(sizeOfBoneOffsets, 1, iStream, isBigEndianness);

    for (int i = 0; i < sizeOfBoneOffsets; i++) {

        Byte boneOffsetSizeOfName;

        readBytes(boneOffsetSizeOfName, 1, iStream, isBigEndianness);

        if (boneOffsetSizeOfName > 0) {
            const char* boneOffsetName = new char[boneOffsetSizeOfName];

            readBytes(boneOffsetName, boneOffsetSizeOfName, iStream);

            //this is address of, so I dont have to use temporary mat4 here.
            // by using the [] notation, we create a new offset if it doesn't already exist.
            glm::mat4& offset = outputAnimation.boneOffset[std::string(boneOffsetName, boneOffsetSizeOfName)];

            for (int j = 0; j < Mat_4_ROW_COL_SIZE; j++) {
                for (int k = 0; k < Mat_4_ROW_COL_SIZE; k++) {
                    readBytes(offset[j][k], SIZE_OF_FLOAT, iStream, isBigEndianness);
                }
            }

            delete[] boneOffsetName;
            boneOffsetName = nullptr;

        }
#ifdef DEBUG
        else {
            printf("There was an error reading the bone offsets. BoneOffsetSizeOfName is less than or equal to zero.  (_3DMS::_3DM_IO::readAnimation) \n");
        }
#endif
    }

    { //scoped for organization

        Byte sizeOfBoneTransforms = 0;
        readBytes(sizeOfBoneTransforms, 1, iStream, isBigEndianness);

        outputAnimation.boneTransformations.resize(sizeOfBoneTransforms);

        for (int i = 0; i < sizeOfBoneTransforms; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    readBytes(outputAnimation.boneTransformations[i][j][k], SIZE_OF_FLOAT, iStream, isBigEndianness);
                }
            }
        }
    }

    return outputAnimation;
}

void _3DM::_3DM_IO::writeMesh(_3DM::Mesh& mesh, std::ofstream& oStream) {

    { //scoped for organization

        FourBytes amountOfVertices = static_cast<FourBytes>(mesh.vertices.size()); //
        FourBytes amountOfNormals  = static_cast<FourBytes>(mesh.normals.size()); //
        FourBytes amountOfUvs      = static_cast<FourBytes>(mesh.uvs.size()); //
        FourBytes amountOfIndices  = static_cast<FourBytes>(mesh.indices.size()); //

        writeBytes(amountOfVertices, 4, oStream);
        writeBytes(amountOfNormals, 4, oStream);
        writeBytes(amountOfUvs, 4, oStream);
        writeBytes(amountOfIndices, 4, oStream);

        writeGLMVec<glm::vec3>(oStream, mesh.vertices, amountOfVertices, SIZE_OF_FLOAT);
        writeGLMVec<glm::vec3>(oStream, mesh.normals, amountOfNormals, SIZE_OF_FLOAT);
        writeGLMVec<glm::vec2>(oStream, mesh.uvs, amountOfUvs, SIZE_OF_FLOAT);

        for (unsigned int i = 0; i < amountOfIndices; i++) {
            writeBytes(mesh.indices[i], 4, oStream);
        }
    }

    //One byte because there shouldn't be more than 255 textures. even 5 textures is pushing it.
    Byte amountOfTextures = static_cast<Byte>(mesh.textures.size());
    Byte sizeOfName       = static_cast<Byte>(mesh.name.size());

    writeBytes(amountOfTextures, 1, oStream);

    for (int i = 0; i < amountOfTextures; i++) {
        Byte imagePathSize = static_cast<Byte>(mesh.textures[i].imagePath.size());
        Byte imageType     = static_cast<Byte>(mesh.textures[i].imageType);

        writeBytes(imagePathSize, 1, oStream);
        writeBytes(mesh.textures[i].imagePath.c_str(), imagePathSize, oStream);
        writeBytes(imageType, 1, oStream);
    }

    for (unsigned int i = 0; i < Mat_4_ROW_COL_SIZE; i++) {
        for (unsigned int j = 0; j < Mat_4_ROW_COL_SIZE; j++) {
            writeBytes(mesh.baseModelMatrix[i][j], SIZE_OF_FLOAT, oStream);
        }
    }

    writeBytes(sizeOfName, 1, oStream);

    if (sizeOfName > 0) {
        writeBytes(mesh.name.c_str(), sizeOfName, oStream);
    }
}

_3DM::Mesh _3DM::_3DM_IO::readMesh(std::ifstream& iStream) {
    _3DM::Mesh newMesh;

    FourBytes amountOfVertices = 0;
    FourBytes amountOfNormals  = 0;
    FourBytes amountOfUvs      = 0;
    FourBytes amountOfIndices  = 0;

    Byte amountOfTextures = 0;

    Byte sizeOfName = 0;

    const char* newName = nullptr;

    readBytes(amountOfVertices, 4, iStream, isBigEndianness);
    readBytes(amountOfNormals, 4, iStream, isBigEndianness);
    readBytes(amountOfUvs, 4, iStream, isBigEndianness);
    readBytes(amountOfIndices, 4, iStream, isBigEndianness);

    newMesh.vertices = readGLMVec<glm::vec3>(iStream, amountOfVertices, SIZE_OF_FLOAT);
    newMesh.normals  = readGLMVec<glm::vec3>(iStream, amountOfNormals, SIZE_OF_FLOAT);
    newMesh.uvs      = readGLMVec<glm::vec2>(iStream, amountOfUvs, SIZE_OF_FLOAT);

    newMesh.indices.resize(amountOfIndices);
    for (unsigned int i = 0; i < amountOfIndices; i++) {
        readBytes(newMesh.indices[i], 4, iStream, isBigEndianness);
    }

    readBytes(amountOfTextures, 1, iStream, isBigEndianness);

    newMesh.textures.resize(amountOfTextures);

    for (int i = 0; i < amountOfTextures; i++) {
        Byte imagePathSize;

        const char* imagePath = nullptr;

        readBytes(imagePathSize, 1, iStream, isBigEndianness);

        imagePath = new char[imagePathSize];

        readBytes(imagePath, imagePathSize, iStream);

        readBytes(newMesh.textures[i].imageType, 1, iStream, isBigEndianness);

        newMesh.textures[i].imagePath = std::string(imagePath, imagePathSize);

        delete[] imagePath;
        imagePath = nullptr;
    }

    for (unsigned int i = 0; i < Mat_4_ROW_COL_SIZE; i++) {
        for (unsigned int j = 0; j < Mat_4_ROW_COL_SIZE; j++) {
            readBytes(newMesh.baseModelMatrix[i][j], SIZE_OF_FLOAT, iStream, isBigEndianness);
        }
    }

    readBytes(sizeOfName, 1, iStream, isBigEndianness);

    if (sizeOfName > 0) {

        newName = new char[sizeOfName];
        iStream.read(const_cast<char*>(newName), sizeOfName);

        newMesh.name = std::string(newName, sizeOfName);

        delete[] newName;
        newName = nullptr;
    }

    return newMesh;
}

void _3DM::_3DM_IO::writeAnimatedMesh(_3DM::AnimatedMesh& animatedMesh, std::ofstream& oStream) {
    FourBytes amountOfWeights = static_cast<FourBytes>(animatedMesh.weights.size()); //Four bytes, because there might be a lot of these.
    FourBytes amountOfIds     = static_cast<FourBytes>(animatedMesh.boneIDs.size()); //

    writeBytes(amountOfWeights, 4, oStream);
    writeBytes(amountOfIds, 4, oStream);

    writeGLMVec<glm::vec4>(oStream, animatedMesh.weights, amountOfWeights, SIZE_OF_FLOAT);
    writeGLMVec<glm::vec4>(oStream, animatedMesh.boneIDs, amountOfIds, SIZE_OF_FLOAT);
    writeMesh(animatedMesh.mesh, oStream);
}

_3DM::AnimatedMesh _3DM::_3DM_IO::readAnimatedMesh(std::ifstream& iStream) {
    _3DM::AnimatedMesh newMesh;

    FourBytes amountOfWeights = 0;
    FourBytes amountOfIds     = 0;

    readBytes(amountOfWeights, 4, iStream, isBigEndianness);
    readBytes(amountOfIds, 4, iStream, isBigEndianness);

    newMesh.weights = readGLMVec<glm::vec4>(iStream, amountOfWeights, SIZE_OF_FLOAT);
    newMesh.boneIDs = readGLMVec<glm::vec4>(iStream, amountOfIds, SIZE_OF_FLOAT);
    newMesh.mesh    = readMesh(iStream);

    return newMesh;
}

void _3DM::_3DM_IO::writeAnimationClip(_3DM::AnimationClip& animClip, std::ofstream& oStream) {
    Byte sizeOfName = static_cast<Byte>(animClip.name.size());

    oStream.write(reinterpret_cast<char*>(&sizeOfName), 1);
    oStream.write(animClip.name.c_str(), sizeOfName);

    oStream.write(reinterpret_cast<char*>(&animClip.startTime), SIZE_OF_FLOAT);
    oStream.write(reinterpret_cast<char*>(&animClip.endTime), SIZE_OF_FLOAT);
}

_3DM::AnimationClip _3DM::_3DM_IO::readAnimationClip(std::ifstream& iStream) {
    _3DM::AnimationClip newAnimationClip;

    Byte sizeOfName = 0;

    readBytes(sizeOfName, 1, iStream, isBigEndianness);

    if (sizeOfName > 0) {
        const char* name = new char[sizeOfName];
        iStream.read(const_cast<char*>(name), sizeOfName);
        newAnimationClip.name = std::string(name, sizeOfName);
        delete[] name;
        name = nullptr;
    }
#ifdef DEBUG
    else {
        printf("Animation clip has no name! (_3DMS::_3DM_IO::ReadAnimationClip)\n");
    }
#endif

    readBytes(newAnimationClip.startTime, SIZE_OF_FLOAT, iStream, isBigEndianness);
    readBytes(newAnimationClip.endTime, SIZE_OF_FLOAT, iStream, isBigEndianness);

    return newAnimationClip;
}

void _3DM::_3DM_IO::setIfstream(const char* path) {
    iStream = std::ifstream(path, std::ios::in | std::ios::binary);
    iStream.seekg(0, iStream.beg);
}

void _3DM::_3DM_IO::writeAnimatedModel(_3DM::AnimatedModel& model, std::ofstream& oStream) {

    Byte boneIdSize      = static_cast<Byte>(model.boneIDMap.size());
    Byte sizeOfRootPath  = static_cast<Byte>(model.rootPath.size());
    Byte sizeOfSignature = static_cast<Byte>(_3DMA_Signature.size());

    writeBytes(sizeOfSignature, 1, oStream);

    oStream.write(_3DMA_Signature.c_str(), sizeOfSignature);

    writeAnimation(model.modelsAnimation, oStream);

    writeVector<_3DM::AnimatedMesh>([&](_3DM::AnimatedMesh& mesh, std::ofstream& oS) { writeAnimatedMesh(mesh, oS); }, model.meshes, oStream);
    writeVector<_3DM::AnimationClip>([&](_3DM::AnimationClip& mesh, std::ofstream& oS) { writeAnimationClip(mesh, oS); }, model.animationClips, oStream);

    writeBytes(boneIdSize, 1, oStream);

    for (std::map<std::string, FourBytes>::iterator i = model.boneIDMap.begin(); i != model.boneIDMap.end(); ++i) {
        Byte sizeOfName = static_cast<Byte>(i->first.size());

        writeBytes(sizeOfName, 1, oStream);
        oStream.write(const_cast<char*>(i->first.c_str()), sizeOfName);

        writeBytes(i->second, 4, oStream);
    }

    writeBytes(model.modelLoaded, 1, oStream);

    writeBytes(sizeOfRootPath, 1, oStream);

    oStream.write(model.rootPath.c_str(), sizeOfRootPath);
}

_3DM::AnimatedModel _3DM::_3DM_IO::readAnimatedModel(const std::string& path) {

    isBigEndianness = isBigEndian();

    _3DM::AnimatedModel newModel;

    Byte boneIdSize      = 0;
    Byte sizeOfRootPath  = 0;
    Byte sizeOfSignature = 0;

    setIfstream(path.c_str());

    const char* rootPath = nullptr;

    readBytes(sizeOfSignature, 1, iStream, isBigEndianness);

    iStream.seekg(static_cast<Byte>(iStream.tellg()) + sizeOfSignature, std::ios::beg);

    newModel.modelsAnimation = readAnimation(iStream);

    newModel.meshes         = readVector<_3DM::AnimatedMesh>([&]() -> _3DM::AnimatedMesh { return readAnimatedMesh(iStream); }, iStream);
    newModel.animationClips = readVector<_3DM::AnimationClip>([&]() -> _3DM::AnimationClip { return readAnimationClip(iStream); }, iStream);

    readBytes(boneIdSize, 1, iStream, isBigEndianness);

    for (int i = 0; i < boneIdSize; i++) {
        Byte sizeOfName = 0;

        FourBytes id;

        const char* newStr = nullptr;

        readBytes(sizeOfName, 1, iStream, isBigEndianness);

        newStr = new char[sizeOfName];

        iStream.read(const_cast<char*>(newStr), sizeOfName);

        readBytes(id, 4, iStream, isBigEndianness);

        newModel.boneIDMap[std::string(newStr, sizeOfName)] = id;

        delete[] newStr;

        newStr = nullptr;
    }

    readBytes(newModel.modelLoaded, 1, iStream, isBigEndianness);

    readBytes(sizeOfRootPath, 1, iStream, isBigEndianness);

    rootPath = new char[sizeOfRootPath];

    iStream.read(const_cast<char*>(rootPath), sizeOfRootPath);

    newModel.rootPath = std::string(rootPath, sizeOfRootPath);

    delete[] rootPath;
    rootPath = nullptr;

    newModel.modelLoaded = true;

    return newModel;
}

void _3DM::_3DM_IO::writeModel(_3DM::Model& model, std::ofstream& oStream) {
    writeVector<_3DM::Mesh>([&](_3DM::Mesh& mesh, std::ofstream& oStream) { writeMesh(mesh, oStream); }, model.meshes, oStream);

    TwoBytes sizeOfString = static_cast<TwoBytes>(model.rootPath.size());
    writeBytes(sizeOfString, 2, oStream);
    writeBytes(model.rootPath.c_str(), sizeOfString, oStream);

    writeBytes(model.modelLoaded, 1, oStream);
}

_3DM::Model _3DM::_3DM_IO::readModel(const std::string& path) {

    _3DM::Model newModel;
    TwoBytes sizeOfString;

    setIfstream(path.c_str());

    newModel.meshes = readVector<_3DM::Mesh>([&]() -> _3DM::Mesh { return readMesh(iStream); }, iStream);

    readBytes(sizeOfString, 2, iStream, isBigEndianness);
    const char* rootPath = new char[sizeOfString];
    readBytes(rootPath, sizeOfString, iStream);

    newModel.rootPath = std::string(rootPath, sizeOfString);

    readBytes(newModel.modelLoaded, 1, iStream, isBigEndianness);

    newModel.modelLoaded = true;

    delete[] rootPath;
    rootPath = nullptr;

    return newModel;
}

_3DM::_3DM_IO::~_3DM_IO() {
}
