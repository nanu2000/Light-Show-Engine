#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H

#include "Camera.h"
#include "DebuggingController.h"
#include "DirectionalLightShadowMap.h"
#include "GuiButton.h"
#include "GuiSprite.h"
#include "GuiString.h"
#include "Model.h"
#include "PointLightShadowMap.h"
#include "Quad.h"
#include "RenderTexture.h"
#include "Scene.h"
#include "Shader.h"
#include "SystemBase.h"

using _3DM::AnimatedModel;
using _3DM::Model;

class RenderingSystem : public SystemBase {

public:
    void initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) override;
    void render(PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap, Time& currentTime, const RenderTexture& renderTexture);

private:
    void
    renderAll(Camera& currentCamera, Time& currentTime, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap);
    void renderDebugging(Camera& currentCamera);
    void renderModels(Camera& currentCamera, Time& currentTime, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap);
    void renderOthers(Camera& currentCamera, Time& currentTime);
    void renderParticles(Particles& particles, Camera& currentCamera, Time& currentTime);

    void initializeLights(Shader& litShader);
    void initializeModels(Shader& shader, const int32_t& entity);

    //Prepares and retrieves the first shader found in scene that is associated with entity.
    //**Does use program.
    Shader* prepareShader(const int32_t& entity, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap, Camera& currentCamera, Time& currentTime);

    void supplyLitShaderUniforms(Shader& shader, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap, Camera& currentCamera, Time& currentTime);

    void supplyDefaultShaderUniforms(Shader& shader, Camera& currentCamera, Time& currentTime);

    void supplyParticleShaderUniforms(Shader& particleShader, Camera& currentCamera, Time& currentTime);

    Quad screenQuad;
    Shader screenShader;

    //Todo: move to a debugging component/system.
    glm::mat4 depthMatrix = glm::mat4(1.0f);
    Quad depthQuad;
    Shader depthShader;
};
#endif
