#ifndef BONE_COLLISION_MESH_H
#define BONE_COLLISION_MESH_H
#include "CollisionMesh.h"
#include "AnimatedModel.h"

const float DEFAULT_BONE_COLLIDER_SCALE = .2f;

class BoneCollisionMesh : public Component<BoneCollisionMesh>
{

public:

	BoneCollisionMesh() {}
	
	template<typename T>
	void addCollider(uint32_t boneID, const CollisionTag& cTag, const T & shape, const glm::vec3 & scale = glm::vec3(DEFAULT_BONE_COLLIDER_SCALE))
	{

		colliders[boneID].initialize
		(
			btTransform(btQuaternion(btVector3(1, 0, 0), glm::radians(0.0f)), btVector3(0, 0, 0)),
			shape,
			1.f,
			cTag,
			true,
			btVector3(0, 0, 0),
			0.0f,
			0.f,
			true
		);

		colliders[boneID].setScale(scale);

		colliders[boneID].setEntityID(cTag.entity);

		colliders[boneID].getRigidBody()->setActivationState(DISABLE_DEACTIVATION);

		collisionTags.push_back(colliders[boneID].getTag());

	}

	template<typename T>
	void addCollider(uint32_t boneID, uint32_t entityID, COLLISION_TAGS tag, const T & shape, const glm::vec3 & scale = glm::vec3(DEFAULT_BONE_COLLIDER_SCALE))
	{
		addCollider(boneID, CollisionTag(tag, entityID), shape, scale);
	}

	template<typename func>
	void iterateThroughColliders(func function)
	{
		for (std::map<int32_t, CollisionMesh>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		{
			function(it->second, it->first);
		}
	}



	void addCollider(uint32_t boneID, uint32_t entityID, COLLISION_TAGS tag,	const glm::vec3 & scale = glm::vec3(DEFAULT_BONE_COLLIDER_SCALE));
	void addCollider(uint32_t boneID, const CollisionTag& cTag,					const glm::vec3 & scale = glm::vec3(DEFAULT_BONE_COLLIDER_SCALE));



	void setCollisionTag	(uint32_t boneID, const CollisionTag& cTag);
	void setScale			(const glm::vec3 & scale, uint32_t boneID);
	void setOffset			(const glm::vec3 & offset, uint32_t boneID);

	void		getColliderOffset(int32_t boneID, glm::vec3& output) const;
	glm::vec3	getColliderOffset(int32_t boneID) const;

	inline const std::vector<const CollisionTag*> * getCollisionTags() { return &collisionTags; }

private:

	std::map		<int32_t, glm::vec3>		boneOffsets;
	std::map		<int32_t, CollisionMesh>	colliders;	
	std::vector		<const CollisionTag *>		collisionTags;

};








#endif