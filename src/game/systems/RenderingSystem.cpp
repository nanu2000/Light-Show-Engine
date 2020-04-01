#include "RenderingSystem.h"

void RenderingSystem::initialize(Scene& scene, Engine::SystemVitals& sv, SubSystems& ssystems) {
    currentScene = &scene;
    systemVitals = &sv;
    systems      = &ssystems;

    currentScene->loopEntities([&](const Scene::Entity& entity) {
        if (SkyBox* skyBox = currentScene->getComponent<SkyBox>(entity.id)) {
            systems->skyBoxSystem.init(*skyBox);
        }
        if (Shader* shader = currentScene->getComponent<Shader>(entity.id)) {

            if (shader->getShaderType() == SHADER_TYPE::Lit) {

                initializeLights(*shader, sv);
                initializeModels(*shader, entity.id);

                if (Material* mat = currentScene->getComponent<Material>(entity.id)) {
                    shader->setMaterial(*mat);
                }
                if (SimpleMaterial* mat = currentScene->getComponent<SimpleMaterial>(entity.id)) {
                    shader->setSimpleMaterial(*mat);
                }
            }

            if (shader->getShaderType() == SHADER_TYPE::Default) {
                initializeModels(*shader, entity.id);
            }
        }

        return false;
    });

    screenShader = ShaderLocator::getService().getShader("screen", "assets/shaders/render-texture.vert", "assets/shaders/render-texture-ms.frag", SHADER_TYPE::Default);
}

void RenderingSystem::initializeLights(Shader& litShader, Engine::SystemVitals& sv) {

    Settings& currentSettings = sv.getSettings();

    const std::vector<PointLight*> pointLights = currentScene->getAllComponentsOfType<PointLight>();

    //Right now I'm only using 1 dir light
    const DirectionalLight* directionalLight = currentScene->getFirstActiveComponentOfType<DirectionalLight>();

    litShader.useProgram();

    if (directionalLight) {
        litShader.setDirectionalLight(*directionalLight);
    }

    //We use point index because the light might not be active.
    int pointIndex = 0;

    for (unsigned int i = 0; i < pointLights.size(); i++) {
        if (pointLights[i]->isActive()) {
            if (pointIndex < currentSettings.getLightsPerEntity()) {
                litShader.setPointLight(currentSettings, *pointLights[i], pointIndex);
                pointIndex++;
            } else {
                break;
            }
        }
    }
}

void RenderingSystem::initializeModels(Shader& shader, const int32_t& entity) {
    if (_3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity)) {
        animatedModel->initialize(shader);
    }
    if (_3DM::Model* model = currentScene->getComponent<_3DM::Model>(entity)) {
        model->initialize(shader);
    }
}

void RenderingSystem::render(Engine::SystemVitals& sv) {

    /******************************************|
    |*		The Current Rendering Order		  *|
    |*----------------------------------------*|
    |*First, render All Opaque Objects.		  *|
    |*Last, Render All GUI Objects.			  *|
    |******************************************/

    PointLightShadowMap& pointShadowMap             = sv.getPointShadowMap();
    DirectionalLightShadowMap& directionalShadowMap = sv.getDirectionalShadowMap();
    Time& currentTime                               = sv.getTime();
    PhysicsWorld& physics                           = sv.getPhysicsWorld();
    GameState& gameState                            = sv.getGameState();
    RenderTextureMS& renderTexture                  = sv.getRenderTexture();

    if (areVitalsNull()) {
        DBG_LOG("Vitals are null (RenderingSystem.cpp)\n");
        return;
    }
    Camera* currentCamera = currentScene->getFirstActiveComponentOfType<Camera>();

    if (!currentCamera) {
        return;
    }

    const std::vector<Shader*> shaders = currentScene->getAllComponentsOfType<Shader>();

    Shader::setShaderTask(SHADER_TASK::Normal_Render_Task);
    for (unsigned int i = 0; i < shaders.size(); i++) {
        if (shaders.at(i)->getShaderType() != SHADER_TYPE::Lit) {
            continue;
        }

        initializeLights(*shaders.at(i), sv);
    }

    glEnable(GL_DEPTH_TEST);
    //If the point light depth map is active, render to it.
    if (pointShadowMap.isActive()) {

        //Uses shader for depth when getProgramID is called.
        Shader::setShaderTask(SHADER_TASK::Omnidirectional_Depth_Task);

        glViewport(0, 0, pointShadowMap.getDepthMapWidth(), pointShadowMap.getDepthMapHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMap.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);
        renderAll(*currentCamera, sv);
    }

    //If the directional light depth map is active, render to it.
    if (directionalShadowMap.isActive()) {

        //Uses shader for depth when getProgramID is called.
        Shader::setShaderTask(SHADER_TASK::Directional_Depth_Task);

        glViewport(0, 0, directionalShadowMap.getDepthMapWidth(), directionalShadowMap.getDepthMapHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, directionalShadowMap.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);
        renderAll(*currentCamera, sv);
    }

    //Use normal shaders
    Shader::setShaderTask(SHADER_TASK::Normal_Render_Task);

    //Render everything to texture.
    {
        glViewport(0, 0, renderTexture.getWidth(), renderTexture.getHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.getFBO());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderAll(*currentCamera, sv);

        //Multisample :)
        glBlitFramebuffer(0, 0,
                          renderTexture.getWidth(), renderTexture.getHeight(),
                          0, 0, renderTexture.getWidth(), renderTexture.getHeight(),
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    //Render texture to quad.
    {
        glViewport(0, 0, GameInfo::getWindowWidth(), GameInfo::getWindowHeight());
        //don't override getProgramID when it's called.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.useProgram();
        glUniform1i(Shaders::getUniformLocation(screenShader.getProgramID(), Shaders::UniformName::MultisampleCount), renderTexture.getCurrentMultisampleCount());
        screenQuad.render2D(screenShader, renderTexture.getTextureID(), GL_TEXTURE_2D_MULTISAMPLE);
    }
}

void RenderingSystem::renderAll(Camera& currentCamera, Engine::SystemVitals& sv) {

    renderDebugging(currentCamera, sv);
    renderModels(currentCamera, sv);
    renderOthers(currentCamera, sv);
}

void RenderingSystem::renderDebugging(Camera& currentCamera, Engine::SystemVitals& sv) {
    PhysicsWorld& physicsWorld = sv.getPhysicsWorld();

    // Execute debug drawing if enabled
    if (Shader::getShaderTask() != SHADER_TASK::Normal_Render_Task) {
        return;
    }

    const std::vector<PlayerController*> plrctrs = currentScene->getAllComponentsOfType<PlayerController>();
    for (unsigned int i = 0; i < plrctrs.size(); i++) {
        systems->playerControllerSystem.debugRender(physicsWorld, *plrctrs.at(i));
    }

    systems->dayNightCycleSystem.debugRender(physicsWorld);
    systems->debuggingSystem.executeDebugRendering(physicsWorld, *currentCamera.getViewMatrix(), *currentCamera.getProjectionMatrix());
}

void RenderingSystem::renderModels(Camera& currentCamera, Engine::SystemVitals& sv) {

    currentScene->loopEntities([&](const Scene::Entity& entity) {
        if (!entity.isActive) {
            return false;
        }

        ModelBase* modelToRender = nullptr;
        bool isAnimated          = false;

        if (_3DM::Model* model = currentScene->getComponent<_3DM::Model>(entity.id)) {
            modelToRender = model;

        } else if (_3DM::AnimatedModel* animatedModel = currentScene->getComponent<_3DM::AnimatedModel>(entity.id)) {
            modelToRender = animatedModel;
            isAnimated    = true;
        }

        if (!modelToRender) {
            return false;
        }

        if (Shader* shdr = currentScene->getComponent<Shader>(entity.id)) {

            if (shdr->getShaderType() == SHADER_TYPE::Lit) {

                shdr->useProgram();
                supplyLitShaderUniforms(*shdr, currentCamera, sv);
                glUniform1i(Shaders::getUniformLocation(shdr->getProgramID(), Shaders::UniformName::IsModelAnimated), isAnimated);

                modelToRender->renderAll(*shdr);
            }

            if (shdr->getShaderType() == SHADER_TYPE::Default) {

                shdr->useProgram();
                supplyDefaultShaderUniforms(*shdr, currentCamera, sv);

                glUniform1i(Shaders::getUniformLocation(shdr->getProgramID(), Shaders::UniformName::IsModelAnimated), isAnimated);

                modelToRender->renderAll(*shdr);
            }
        }

        return false;
    });
}

// Render Particles and GUI
void RenderingSystem::renderOthers(Camera& currentCamera, Engine::SystemVitals& sv) {

    //Particles and UI aren't 3D so depth render tasks don't need their info.
    if (Shader::getShaderTask() != SHADER_TASK::Normal_Render_Task) {
        return;
    }

    const std::vector<Particles*> particles = currentScene->getAllComponentsOfType<Particles>();

    currentScene->loopEntities([&](const Scene::Entity& entity) {
        SkyBox* skyBox = currentScene->getComponent<SkyBox>(entity.id);
        Shader* shdr   = currentScene->getComponent<Shader>(entity.id);

        if (!skyBox || !shdr) {
            return false;
        }

        if (shdr->getShaderType() != SHADER_TYPE::Default) {
            return false;
        }

        shdr->useProgram();
        supplyDefaultShaderUniforms(*shdr, currentCamera, sv);
        systems->skyBoxSystem.render(*skyBox, currentCamera, *shdr);

        return false;
    });

    for (unsigned int i = 0; i < particles.size(); i++) {
        renderParticles(*particles[i], currentCamera, sv);
    }

    //Only one per scene
    DisplayStatistics* stats = currentScene->getFirstActiveComponentOfType<DisplayStatistics>();
    if (stats) {

        Shader* shader = currentScene->getComponent<Shader>(stats->getEntityID());

        if (shader && shader->getShaderType() == SHADER_TYPE::GUI) {
            shader->useProgram();
            systems->displayStatisticsSystem.render(*shader, *stats);
        }
    }

    //Only one per scene
    PauseMenu* menu = currentScene->getFirstActiveComponentOfType<PauseMenu>();
    if (stats) {

        Shader* shader = currentScene->getComponent<Shader>(menu->getEntityID());

        if (shader && shader->getShaderType() == SHADER_TYPE::GUI) {
            shader->useProgram();
            systems->pauseMenuSystem.render(*shader, *menu);
        }
    }

    //Only one per scene
    DirectionalShadowDebugger* dirShadowDbgr = currentScene->getFirstActiveComponentOfType<DirectionalShadowDebugger>();
    if (dirShadowDbgr) {
        DirectionalLightShadowMap& directionalLightDepthMap = sv.getDirectionalShadowMap();
        systems->directionalShadowDebuggerSystem.render(*dirShadowDbgr, directionalLightDepthMap);
    }
}

void RenderingSystem::renderParticles(Particles& particles, Camera& currentCamera, Engine::SystemVitals& sv) {
    if (Shader* thisShader = currentScene->getComponent<Shader>(particles.getEntityID())) {
        // Check to see if the shader is a particle shader
        if (thisShader->getShaderType() == SHADER_TYPE::Particle) {
            supplyParticleShaderUniforms(*thisShader, currentCamera, sv);

            switch (particles.getParticleType()) {
            case PARTICLE_TYPE::Default:
                systems->defaultParticleSystem.renderParticles(*thisShader, particles);
            default:
                break;
            case PARTICLE_TYPE::Fountain:
                systems->fountainParticleSystem.renderParticles(*thisShader, particles);
                break;
            }
        }
    }
}

Shader* RenderingSystem::prepareShader(const int32_t& entity, Camera& currentCamera, Engine::SystemVitals& sv) {
    Shader* thisShader;

    if (thisShader = currentScene->getComponent<Shader>(entity)) {

        if (thisShader->getShaderType() == SHADER_TYPE::Default) {
            if (currentScene->isEntityActive(entity)) {
                thisShader->useProgram();
                supplyDefaultShaderUniforms(*thisShader, currentCamera, sv);
                return thisShader;
            }
        }

        if (thisShader->getShaderType() == SHADER_TYPE::Lit) {
            if (currentScene->isEntityActive(entity)) {
                thisShader->useProgram();
                supplyLitShaderUniforms(*thisShader, currentCamera, sv);
                return thisShader;
            }
        }
    }

    return nullptr;
}

void RenderingSystem::supplyDefaultShaderUniforms(Shader& shader, Camera& currentCamera, Engine::SystemVitals& sv) {

    const GLint& programID = shader.getProgramID();

    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ViewMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getViewMatrix()));
    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ProjectionMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getProjectionMatrix()));

    glUniform3f(Shaders::getUniformLocation(programID, Shaders::UniformName::ViewPosition), currentCamera.position.x, currentCamera.position.y, currentCamera.position.z);
}

void RenderingSystem::supplyLitShaderUniforms(Shader& shader, Camera& currentCamera, Engine::SystemVitals& sv) {

    PointLightShadowMap& pointLightDepthMap             = sv.getPointShadowMap();
    DirectionalLightShadowMap& directionalLightDepthMap = sv.getDirectionalShadowMap();

    supplyDefaultShaderUniforms(shader, currentCamera, sv);

    glActiveTexture(GL_TEXTURE0 + Shaders::DEPTH_MAP_LOCATION_OMNIDIRECTIONAL);
    glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::PointShadowMap), Shaders::DEPTH_MAP_LOCATION_OMNIDIRECTIONAL);

    //glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::TimeMS), currentTime.sinceStartMS32());

    if (pointLightDepthMap.isActive()) {

        glUniform1f(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::FarPlane), pointLightDepthMap.getFarPlane());
        glUniform3fv(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::LightPosition), 1, &pointLightDepthMap.getCurrentLightPosition()[0]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, pointLightDepthMap.getCubeMap());

    } else {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    glUniformMatrix4fv(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::LightSpaceMatrix), 1, GL_FALSE, glm::value_ptr(*directionalLightDepthMap.getLightSpaceMatrix()));
    glUniform1i(Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::DirectionalShadowMap), Shaders::DEPTH_MAP_LOCATION_DIRECTIONAL);

    glActiveTexture(GL_TEXTURE0 + Shaders::DEPTH_MAP_LOCATION_DIRECTIONAL);
    glBindTexture(GL_TEXTURE_2D, directionalLightDepthMap.getDepthMap());
}

void RenderingSystem::supplyParticleShaderUniforms(Shader& particleShader, Camera& currentCamera, Engine::SystemVitals& sv) {
    particleShader.useProgram();

    const GLint& programID = particleShader.getProgramID();

    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ViewMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getViewMatrix()));
    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ProjectionMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getProjectionMatrix()));
}
