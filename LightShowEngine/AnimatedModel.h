#ifndef MODEL_LOADER
#define MODEL_LOADER
#include "Debug.h"
#include "3DM_Info.h"
#include "SkeletalSystem.h"
#include "AnimatedMesh.h"

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
	class AnimatedModel : public Component<AnimatedModel>, public ModelBase
	{

	public:

		
		AnimatedModel(const std::string& path);

		void initialize(ShaderBase& shader);
		/**************
		*	Getters	  *
		***************/

		glm::mat4 getMeshMatrix				(unsigned int index)				const;
		int getMeshIndex					(const std::string& MeshName)		const;
		int	getAnimationIndex				(const std::string& name)			const;
		int getChannelIndex					(const std::string& channelName)	const;
		int getBoneID						(const std::string& nodeName)		const;
		std::string getBoneName				(uint32_t id) const;
		int amountOfAnimations				() const { return animationClips.size(); }

		glm::mat4	getBoneTransformation	(unsigned int boneId)	const;
		glm::mat4	getBoneTransformationWithoutOffset(unsigned int boneId) const;
		glm::mat4	getBoneTransformationWithoutOffset(const std::string& name) const;

		unsigned int amountOfBones () const { return modelsAnimation.boneTransformations.size(); }
		unsigned int amountOfMeshes() const { return meshes.size(); }

		std::vector<glm::vec3>* getMeshVertices	(unsigned int index);
		std::vector<uint32_t>*	getMeshIndices	(unsigned int index);

		/**************
		*	Setters	  *
		***************/

		void setMeshMatrix					(unsigned int index, const glm::mat4 & newMatrix);
		void setBoneMatrix					(const glm::mat4& transformation, unsigned int boneId);
		void setAnimationClip				(unsigned int clip);


		/************
		*	Other   *
		*************/

		void updateAnimation				();
		void renderAll						(ShaderBase & shader);
		void renderSingleMesh				(unsigned int index, ShaderBase & shader);
		void removeKeyframes				(unsigned int channelIndex);

		void addRotationToKeyFrames			(const glm::quat& rotation, unsigned int channelIndex);
		void addRotationToKeyFrames			(const glm::vec3& rotation, unsigned int channelIndex);
		void overwriteRotationKeyFrames		(const glm::vec3& rotation, unsigned int channelIndex);
		void overwriteRotationKeyFrames		(const glm::quat& rotation, unsigned int channelIndex);
		void addScaleToKeyFrames			(const glm::vec3& scale,	unsigned int channelIndex);
		void overwriteScaleKeyFrames		(const glm::vec3& scale,	unsigned int channelIndex);
		void addPositionToKeyFrames			(const glm::vec3& position, unsigned int channelIndex);
		void overWritePositionToKeyFrames	(const glm::vec3& position, unsigned int channelIndex);

		void setBlendingTime				(const float newTime) { blendingTime = newTime; }

	private:

		struct InterpolatedFrame 
		{
			InterpolatedFrame() : interpolation(0.0f), firstFrame(0), lastFrame(0) {}

			float interpolation = 0;
			int firstFrame		= 0;
			int lastFrame		= 0;
		};


		template <typename T>
		InterpolatedFrame getKeyframesAtTime(std::vector<T>& keys, const float& currentTime)
		{
			InterpolatedFrame thisFrame;

			for (int i = 0; i < static_cast<int>(keys.size()) - 1; i++)
			{
				if (currentTime >= keys[i].time  &&	currentTime < keys.at(i + 1).time)
				{
					thisFrame.interpolation = (currentTime - keys[i].time) / (keys.at(i + 1).time - keys[i].time);
					thisFrame.firstFrame = i;
					thisFrame.lastFrame = i + 1;
					return thisFrame;
				}
			}
			return thisFrame;
		}

		template <typename T>
		unsigned int getNearestFrameAtTime(std::vector<T>& keys, const float& currentTime)
		{
			InterpolatedFrame frames = getKeyframesAtTime(keys, currentTime);

			if (frames.interpolation <= .5f) 
			{
				return frames.firstFrame;
			}
			else
			{
				return frames.lastFrame;
			}
		}



		void initializeBuffers(Mesh & mesh, ShaderBase& shader);
		void initializeTexture(Mesh & mesh, ShaderBase& shader);

		void renderMesh						(unsigned int index, ShaderBase & shader);
		void initializeBoneBuffers			(AnimatedMesh & mesh, ShaderBase& shader);
		void updateBoneTree					(const float& timeInTicks, BoneNode* node, const glm::mat4& parentTransform);
		void blendBoneTree					(const float& lastAnimationTime, _3DM::BoneNode* node, const glm::mat4& parentTransform);

		void bindBuffers					(ShaderBase& shader, AnimatedMesh & mesh);


		Animation								modelsAnimation;
		
		std::vector <AnimatedMesh>				meshes;

		std::map <std::string, uint32_t>		boneIDMap;					//retrieves the bones ID with a given name
		
		uint16_t								currentAnimationClip = 0;
		uint16_t								lastAnimationClip = 0;
		float									timeSinceAnimationStarted = 0;
		float									blendingTime = .1f;
		float									currentBlendingTime = 0;
		float									blendingLastFrameTime = 0;
		uint16_t								blendinglastAnimationClip = 0;
		
		std::vector<AnimationClip>				animationClips;
		
		std::string			rootPath;
		bool				modelLoaded = false;



		AnimatedModel() {}
		friend class _3DM::_3DM_IO;
	
	};

};

#endif