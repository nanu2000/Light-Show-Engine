#include "BoneCollisionMesh.h"

void BoneCollisionMesh::addCollider(uint32_t boneID, uint32_t entityID, COLLISION_TAGS tag, const glm::vec3 & scale)
{
	addCollider(boneID, CollisionTag(tag, entityID), btBoxShape(btVector3(1, 1, 1)), scale);
}

void BoneCollisionMesh::addCollider(uint32_t boneID, const CollisionTag & cTag, const glm::vec3 & scale)
{
	addCollider(boneID, cTag, btBoxShape(btVector3(1, 1, 1)), scale);
}

void BoneCollisionMesh::setCollisionTag(uint32_t boneID, const CollisionTag & cTag)
{
	if (colliders.find(boneID) != colliders.end())
	{
		colliders[boneID].setUserTag(cTag);
	}
	#ifdef DEBUG
	else
	{
		DBG_LOG("Bone ID Goes Out of Bounds (BoneCollisionMesh.cpp)\n");
	}
	#endif
}

void BoneCollisionMesh::setScale(const glm::vec3 & scale, uint32_t boneID)
{
	if (colliders.find(boneID) != colliders.end())
	{
		colliders[boneID].setScale(scale);
	}
	#ifdef DEBUG
	else
	{
		DBG_LOG("Bone ID Goes Out of Bounds (BoneCollisionMesh.cpp)\n");
	}
	#endif
}


void BoneCollisionMesh::setOffset(const glm::vec3 & offset, uint32_t boneID)
{
	if (colliders.find(boneID) != colliders.end())
	{
		boneOffsets[boneID] = offset;
	}
	#ifdef DEBUG
	else
	{
		DBG_LOG("Bone ID Goes Out of Bounds (BoneCollisionMesh.cpp)\n");
	}
	#endif
}

void BoneCollisionMesh::getColliderOffset(int32_t boneID, glm::vec3 & output) const
{
	output = GameInfo::GLM_VEC3_ZERO;

	std::map<int32_t, glm::vec3>::const_iterator offsetIt = boneOffsets.find(boneID);

	if (offsetIt != boneOffsets.end())
	{
		output = offsetIt->second;
	}
}

glm::vec3 BoneCollisionMesh::getColliderOffset(int32_t boneID) const
{
	std::map<int32_t, glm::vec3>::const_iterator offsetIt = boneOffsets.find(boneID);

	if (offsetIt != boneOffsets.end())
	{
		return offsetIt->second;
	}
	return GameInfo::GLM_VEC3_ZERO;
}
