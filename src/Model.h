#ifndef MODEL_H
#define MODEL_H

#include "3DM_Info.h"
#include "Component.h"
#include "Locator.h"
#include "Mesh.h"
#include "ModelBase.h"
#include "ModelSerialization.h"
#include "Shader.h"
#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace _3DM {
    class _3DM_IO;
    class Model : public Component<Model>, public ModelBase {
    public:
        Model(const std::string& path);
        Model() {}
        ~Model();

        void addTexture(const Texture& texture, int index, const _3DM::TextureType& type) {
            if (index >= meshes.size()) {
                DBG_LOG("This index goes out of bounds (_3DM::Model::addTexture)\n");
                return;
            }

            Mesh& mesh = meshes.at(index);
            std::stringstream stringStream;

            ModelTexture modelTexture;
            modelTexture.imageID   = texture.getTextureData();
            modelTexture.imagePath = texture.getLocation();
            modelTexture.imageType = type;

            switch (type) {
            case _3DM::Diffuse:
                mesh.diffuseIndex++;
                stringStream << mesh.diffuseIndex;
                modelTexture.uniformName = Shaders::getUniformName(Shaders::UniformName::DiffuseTexture) + stringStream.str();
                break;
            case _3DM::Specular:
                mesh.specularIndex++;
                stringStream << mesh.specularIndex;
                modelTexture.uniformName = Shaders::getUniformName(Shaders::UniformName::SpecularTexture) + stringStream.str();
                break;
            case _3DM::Normals:
                mesh.normalsIndex++;
                stringStream << mesh.normalsIndex;
                modelTexture.uniformName = Shaders::getUniformName(Shaders::UniformName::NormalTexture) + stringStream.str();
                break;
            }

            mesh.textures.push_back(modelTexture);
        }

        void renderSingleMesh(unsigned int index, Shader& shader);
        int getMeshIndex(const std::string& MeshName) const;
        void initialize(Shader& shader);
        void renderAll(Shader& shader);
        glm::mat4 getMeshMatrix(unsigned int index) const;
        void setMeshMatrix(unsigned int index, const glm::mat4& newMatrix);
        unsigned int amountOfMeshes() { return meshes.size(); }

        std::vector<glm::vec3>* getMeshVertices(unsigned int index);

        std::vector<uint32_t>* getMeshIndices(unsigned int index);

    private:
        std::vector<Mesh> meshes;

        void initializeBuffers(_3DM::Mesh& mesh, Shader& shader);

        void initializeTexture(_3DM::Mesh& mesh, Shader& shader);

        void renderMesh(unsigned int index, Shader& shader);

        std::string rootPath;
        bool modelLoaded = false;
        bool initialized = false;

        friend class _3DM::_3DM_IO;
    };
}

#endif
