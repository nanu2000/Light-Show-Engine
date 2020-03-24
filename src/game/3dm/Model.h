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
        //!Keep in mind that the destructor will call glDelete on openGL objects if the Model was properly initialized.
        Model(const std::string& path);
        ~Model();

        friend void swap(_3DM::Model& first, _3DM::Model& second) // nothrow
        {
            using std::swap;
            swap(first.rootPath, second.rootPath);
            swap(first.modelLoaded, second.modelLoaded);
            swap(first.initialized, second.initialized);
            swap(first.meshes, second.meshes);

            //ModelBase
            swap(first.animatedModel, second.animatedModel);
            swap(first.transform, second.transform);
        }

        Model(const Model&) = delete;
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
        Model() {}
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
