#ifndef BONE_COLLISION_MESH_SYSTEM
#define BONE_COLLISION_MESH_SYSTEM
#include "BoneCollisionMesh.h"
#include "AnimatedModel.h"

class BoneCollisionMeshSystem
{
public:

	void update(BoneCollisionMesh& mesh, const _3DM::AnimatedModel& animatedModel);

};


#endif