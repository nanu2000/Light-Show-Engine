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

    //!The Model controls the lifecycle of a 3D Model. This includes init, rendering, and freeing the data to render the Model.
    class Model : public Component<Model>, public ModelBase {
    public:
        Model(const std::string& path);
        Model() {}
        ~Model();

        friend void swap(_3DM::Model& first, _3DM::Model& second) // nothrow
        {
            std::swap(first.rootPath, second.rootPath);
            std::swap(first.modelLoaded, second.modelLoaded);
            std::swap(first.initialized, second.initialized);
            std::swap(first.meshes, second.meshes);
        }

        Model(Model&& other) noexcept
            : Model() {

            if (other.initialized == true) {

                DBG_LOG("!!Cannot copy Model after initialization due to destructor calling glDelete functions.!!");
            }
            swap(*this, other);
        }

        Model& operator=(Model other) {

            if (other.initialized == true) {
                DBG_LOG("!!Cannot assign Model after initialization due to destructor calling glDelete functions.!!");
            }

            swap(*this, other);
            return *this;
        }

        //! Used to add a texture manually to a mesh.
        void addTexture(const Texture& texture, int meshIndex, const _3DM::TextureType& type);
        void renderSingleMesh(unsigned int index, Shader& shader);
        int getMeshIndex(const std::string& MeshName) const;
        void initialize(Shader& shader);
        void renderAll(Shader& shader);
        void renderMesh(unsigned int index, Shader& shader);
        glm::mat4 getMeshMatrix(unsigned int index) const;
        void setMeshMatrix(unsigned int index, const glm::mat4& newMatrix);
        unsigned int amountOfMeshes() { return meshes.size(); }

        std::vector<glm::vec3>* getMeshVertices(unsigned int index);

        std::vector<uint32_t>* getMeshIndices(unsigned int index);

    private:
        std::vector<Mesh> meshes;

        void initializeBuffers(_3DM::Mesh& mesh, Shader& shader);
        void initializeTexture(_3DM::Mesh& mesh, Shader& shader);

        std::string rootPath;
        bool modelLoaded = false;
        bool initialized = false;

        friend class _3DM::_3DM_IO;
    };
}

#endif
