#ifndef MODEL_LOADER
#define MODEL_LOADER
#include "3DM_Info.h"
#include "AnimatedMesh.h"
#include "Debug.h"
#include "SkeletalSystem.h"

#include "Component.h"
#include "Locator.h"
#include "ModelBase.h"
#include "ModelSerialization.h"
#include "Shader.h"
#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace _3DM {

    class _3DM_IO;

    //!The AnimatedModel controls the lifecycle of a 3D Animated Model. This includes init, rendering, and freeing the data to render the AnimatedModel.
    class AnimatedModel : public Component<AnimatedModel>, public ModelBase {

    public:
        AnimatedModel(const std::string& path);
        ~AnimatedModel();

        //!Initializes the model. Should be called before rendering.
        void initialize(Shader& shader);

        //!Retrieves matrix of mesh at index.
        glm::mat4 getMeshMatrix(unsigned int index) const;

        //!Retrieves the index of a mesh via mesh name.
        int getMeshIndex(const std::string& MeshName) const;

        //!Retrieves animation index of Model via animations name.
        int getAnimationIndex(const std::string& name) const;

        //!Retrieves animation's channel index via channel's name.
        int getChannelIndex(const std::string& channelName) const;

        //!Retrieves id of bone via bones name.
        int getBoneID(const std::string& nodeName) const;

        //!Retrieves bone name via bone's id.
        std::string getBoneName(uint32_t id) const;

        //!Returns amount of animations for the animated model.
        int amountOfAnimations() const { return animationClips.size(); }

        //!Returns the current transform of the bone.
        glm::mat4 getBoneTransformation(unsigned int boneId) const;

        //!Returns the current transform of the bone, without bone's offset matrix.
        glm::mat4 getBoneTransformationWithoutOffset(unsigned int boneId) const;

        //!Returns the current transform of the bone via bone name, without bone's offset matrix.
        glm::mat4 getBoneTransformationWithoutOffset(const std::string& name) const;

        //!Returns the amount of bones in the animated model.
        unsigned int amountOfBones() const { return modelsAnimation.boneTransformations.size(); }

        //!Returns the amount of meshes in the animated model.
        unsigned int amountOfMeshes() const { return meshes.size(); }

        //!Retrieves vertices of mesh at index.
        std::vector<glm::vec3>* getMeshVertices(unsigned int index);

        //!Retrieves indices of mesh at index.
        std::vector<uint32_t>* getMeshIndices(unsigned int index);

        //!Sets matrix of mesh at index.
        void setMeshMatrix(unsigned int index, const glm::mat4& newMatrix);

        //!Sets matrix of a bone at index.
        void setBoneMatrix(const glm::mat4& transformation, unsigned int boneId);

        //!Sets the current animation clip to be ran.
        void setAnimationClip(unsigned int clip);

        //!Used to add a texture manually to a mesh.
        void addTexture(const Texture& texture, int meshIndex, const _3DM::TextureType& type);

        //!Updates the animation a single frame.
        void fixedUpdateAnimation();

        //!Renders all meshes in model.
        void renderAll(Shader& shader);

        //!Renders a mesh at index.
        void renderSingleMesh(unsigned int index, Shader& shader);

        //!Removes a channel via it's index.
        void removeKeyframes(unsigned int channelIndex);

        //!Add rotation to a channel
        void addRotationToKeyFrames(const glm::quat& rotation, unsigned int channelIndex);

        //!Add rotation to a channel
        void addRotationToKeyFrames(const glm::vec3& rotation, unsigned int channelIndex);

        //!Overwrite rotation of a channel
        void overwriteRotationKeyFrames(const glm::vec3& rotation, unsigned int channelIndex);

        //!Overwrite rotation of a channel
        void overwriteRotationKeyFrames(const glm::quat& rotation, unsigned int channelIndex);

        //!Add scale to a channel
        void addScaleToKeyFrames(const glm::vec3& scale, unsigned int channelIndex);

        //!Overwrite scale of a channel
        void overwriteScaleKeyFrames(const glm::vec3& scale, unsigned int channelIndex);

        //!Add position to a channel
        void addPositionToKeyFrames(const glm::vec3& position, unsigned int channelIndex);

        //!Overwrite position of a channel
        void overWritePositionToKeyFrames(const glm::vec3& position, unsigned int channelIndex);

        //!Set's amount of time it takes to interpolate one animation to another
        void setBlendingTime(const float newTime) { blendingTime = newTime; }

    private:
        //!Used to interpolate two frames for smooth animation transitions.
        struct InterpolatedFrame {
            InterpolatedFrame()
                : interpolation(0.0f)
                , firstFrame(0)
                , lastFrame(0) {}

            float interpolation = 0;
            int firstFrame      = 0;
            int lastFrame       = 0;
        };

        template <typename T>
        //!Returns interpolated keyframes at a given time.
        InterpolatedFrame getKeyframesAtTime(std::vector<T>& keys, float currentTime) {
            InterpolatedFrame thisFrame;

            for (int i = 0; i < static_cast<int>(keys.size()) - 1; i++) {
                if (currentTime >= keys[i].time && currentTime < keys.at(i + 1).time) {
                    thisFrame.interpolation = (currentTime - keys[i].time) / (keys.at(i + 1).time - keys[i].time);
                    thisFrame.firstFrame    = i;
                    thisFrame.lastFrame     = i + 1;
                    return thisFrame;
                }
            }
            return thisFrame;
        }

        template <typename T>
        //!Retrieves the nearest keyframe based on the current time.
        unsigned int getNearestFrameAtTime(std::vector<T>& keys, float currentTime) {
            InterpolatedFrame frames = getKeyframesAtTime(keys, currentTime);

            if (frames.interpolation <= .5f) {
                return frames.firstFrame;
            } else {
                return frames.lastFrame;
            }
        }

        //!Initializes the buffers for a mesh.
        void initializeBuffers(Mesh& mesh, Shader& shader);

        //!Initializes the textures for a mesh.
        void initializeTexture(Mesh& mesh, Shader& shader);

        //!Renders a mesh at index.
        void renderMesh(unsigned int index, Shader& shader);

        //!Initializes the bone buffers for a mesh.
        void initializeBoneBuffers(AnimatedMesh& mesh, Shader& shader);

        //!This function will recursively update the bonetree according to the time given. it will also interpolate properly between each keyframe.
        void updateBoneTree(const float& timeInTicks, BoneNode* node, const glm::mat4& parentTransform);

        //!This function will recursively blend the bonetree according to the time given. it will also interpolate properly between each keyframe.
        void blendBoneTree(const float& lastAnimationTime, _3DM::BoneNode* node, const glm::mat4& parentTransform);

        //!Bind's buffers before rendering
        void bindBuffers(Shader& shader, AnimatedMesh& mesh);

        //!The animation of the model
        Animation modelsAnimation;

        //!The meshes of the model
        std::vector<AnimatedMesh> meshes;

        //!The boneId's mapped to the bone name
        std::map<std::string, uint32_t> boneIDMap;

        uint16_t currentAnimationClip      = 0;
        uint16_t lastAnimationClip         = 0;
        float timeSinceAnimationStarted    = 0;
        float blendingTime                 = .1f;
        float currentBlendingTime          = 0;
        float blendingLastFrameTime        = 0;
        uint16_t blendinglastAnimationClip = 0;

        //!Animation clips for the AnimatedModel
        std::vector<AnimationClip> animationClips;

        std::string rootPath;
        bool modelLoaded = false;
        bool initialized = false;

        AnimatedModel() {}
        friend class _3DM::_3DM_IO;
    };

};

#endif
