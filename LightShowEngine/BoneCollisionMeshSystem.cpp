#include "BoneCollisionMeshSystem.h"

void BoneCollisionMeshSystem::update(BoneCollisionMesh& mesh, const _3DM::AnimatedModel& animatedModel) {
    mesh.iterateThroughColliders(
        [&](CollisionMesh& currentCollider, int32_t boneID) {
            glm::mat4 transformation;
            btTransform newTransform;

            transformation = glm::translate(transformation, animatedModel.transform.position);
            transformation = glm::rotate(transformation, glm::angle(animatedModel.transform.rotation), glm::axis(animatedModel.transform.rotation));
            transformation = glm::scale(transformation, animatedModel.transform.scale);

            transformation = glm::translate(transformation * animatedModel.getBoneTransformationWithoutOffset(boneID), mesh.getColliderOffset(boneID));

            newTransform.setFromOpenGLMatrix(glm::value_ptr(transformation));

            currentCollider.getMotionState()->setWorldTransform(newTransform);
        });
}
