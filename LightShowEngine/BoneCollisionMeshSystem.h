#ifndef BONE_COLLISION_MESH_SYSTEM
#define BONE_COLLISION_MESH_SYSTEM
#include "AnimatedModel.h"
#include "BoneCollisionMesh.h"

class BoneCollisionMeshSystem {
public:
    void update(BoneCollisionMesh& mesh, const _3DM::AnimatedModel& animatedModel);
};

#endif
