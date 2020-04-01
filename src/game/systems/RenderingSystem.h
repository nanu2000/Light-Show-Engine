#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H

#include "Camera.h"
#include "DirectionalLightShadowMap.h"
#include "GuiButton.h"
#include "GuiSprite.h"
#include "GuiString.h"
#include "MainSystemBase.h"
#include "Model.h"
#include "PointLightShadowMap.h"
#include "Quad.h"
#include "RenderTexture.h"
#include "Scene.h"
#include "Shader.h"

using _3DM::AnimatedModel;
using _3DM::Model;

class RenderingSystem : public MainSystemBase {

public:
    ~RenderingSystem() {}
    void initialize(Scene& scene, Engine::SystemVitals& systemVitals, SubSystems& ssystems) override;
    void render(Engine::SystemVitals& systemVitals);

private:
    void
    renderAll(Camera& currentCamera, Engine::SystemVitals& sv);
    void renderDebugging(Camera& currentCamera, Engine::SystemVitals& sv);
    void renderModels(Camera& currentCamera, Engine::SystemVitals& sv);
    void renderOthers(Camera& currentCamera, Engine::SystemVitals& sv);
    void renderParticles(Particles& particles, Camera& currentCamera, Engine::SystemVitals& sv);

    void initializeLights(Shader& litShader, Engine::SystemVitals& sv);
    void initializeModels(Shader& shader, const int32_t& entity);

    //!Prepares and retrieves the first shader found in scene that is associated with entity.
    //!*Does use program.
    Shader* prepareShader(const int32_t& entity, Camera& currentCamera, Engine::SystemVitals& sv);

    void supplyLitShaderUniforms(Shader& shader, Camera& currentCamera, Engine::SystemVitals& sv);

    void supplyDefaultShaderUniforms(Shader& shader, Camera& currentCamera, Engine::SystemVitals& sv);

    void supplyParticleShaderUniforms(Shader& particleShader, Camera& currentCamera, Engine::SystemVitals& sv);

    //! Used to render the scene to a quad.
    Quad screenQuad;

    //! The shader used for the screenQuad.
    Shader screenShader;
};
#endif
