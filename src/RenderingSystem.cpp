#include "RenderingSystem.h"

void RenderingSystem::initialize(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& ssystems) {

    currentScene    = &scene;
    currentSettings = &settings;
    physicsWorld    = &world;
    systems         = &ssystems;

    currentScene->loopEntities([&](const Scene::Entity& entity) {
        if (SkyBox* skyBox = currentScene->getComponent<SkyBox>(entity.id)) {
            systems->skyBoxSystem.init(*skyBox);
        }
        if (Shader* shader = currentScene->getComponent<Shader>(entity.id)) {

            if (shader->getShaderType() == SHADER_TYPE::Lit) {

                initializeLights(*shader);
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

    screenShader = ShaderBase("assets/shaders/render-texture.vert", "assets/shaders/render-texture.frag", SHADER_TYPE::Default);
    depthShader  = ShaderBase("assets/shaders/depth.vert", "assets/shaders/depth.frag", SHADER_TYPE::Default);

    //Todo: move to a debugging component/system.
    //Pretty much we render a neat little quad with the directional light's depth map in it for debugging purposes. This will be useful for finetuning shaders.
    depthMatrix *= glm::translate(glm::vec3(0.8f, 0.8f, 0.0f));
    depthMatrix *= glm::scale(glm::vec3(0.2, 0.2, 0.2));
}

void RenderingSystem::initializeLights(Shader& litShader) {

    const std::vector<PointLight*>& pointLights = currentScene->getAllComponentsOfType<PointLight>();

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
            if (pointIndex < currentSettings->getLightsPerEntity()) {
                litShader.setPointLight(*currentSettings, *pointLights[i], pointIndex);
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

void RenderingSystem::render(PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap, Time& currentTime, const RenderTexture& renderTexture) {

    /******************************************|
    |*		The Current Rendering Order		  *|
    |*----------------------------------------*|
    |*First, render All Opaque Objects.		  *|
    |*Last, Render All GUI Objects.			  *|
    |******************************************/

    if (areVitalsNull()) {
        DBG_LOG("Vitals are null (RenderingSystem.cpp)\n");
        return;
    }

    Camera* currentCamera = currentScene->getFirstActiveComponentOfType<Camera>();

    if (!currentCamera) {
        return;
    }

    const std::vector<Shader*>& shaders = currentScene->getAllComponentsOfType<Shader>();

    ShaderBase::setShaderTask(SHADER_TASK::Normal_Render_Task);
    for (unsigned int i = 0; i < shaders.size(); i++) {
        if (shaders.at(i)->getShaderType() != SHADER_TYPE::Lit) {
            continue;
        }

        initializeLights(*shaders.at(i));
    }

    glEnable(GL_DEPTH_TEST);
    //If the point light depth map is active, render to it.
    if (pointLightDepthMap.isActive()) {

        //Uses shader for depth when getProgramID is called.
        ShaderBase::setShaderTask(SHADER_TASK::Omnidirectional_Depth_Task);

        glViewport(0, 0, pointLightDepthMap.getDepthMapWidth(), pointLightDepthMap.getDepthMapHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, pointLightDepthMap.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);
        renderAll(*currentCamera, currentTime, pointLightDepthMap, directionalLightDepthMap);
    }

    //If the directional light depth map is active, render to it.
    if (directionalLightDepthMap.isActive()) {

        //Uses shader for depth when getProgramID is called.
        ShaderBase::setShaderTask(SHADER_TASK::Directional_Depth_Task);

        glViewport(0, 0, directionalLightDepthMap.getDepthMapWidth(), directionalLightDepthMap.getDepthMapHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, directionalLightDepthMap.getFBO());
        glClear(GL_DEPTH_BUFFER_BIT);
        renderAll(*currentCamera, currentTime, pointLightDepthMap, directionalLightDepthMap);
    }

    //Use normal shaders
    ShaderBase::setShaderTask(SHADER_TASK::Normal_Render_Task);

    //Render everything to texture.
    {
        glViewport(0, 0, renderTexture.getWidth(), renderTexture.getHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.getFBO());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderAll(*currentCamera, currentTime, pointLightDepthMap, directionalLightDepthMap);

        //Todo: move this into a ui sys/component for debugging
        //Pretty much we render a neat little quad with the directional light's depth map in it for debugging purposes. This will be useful for finetuning shaders.

        depthShader.useProgram();
        glUniform3f(Shaders::getUniformLocation(depthShader.getProgramID(), Shaders::UniformName::ViewPosition), currentCamera->position.x, currentCamera->position.y, currentCamera->position.z);
        depthQuad.render3D(depthShader, directionalLightDepthMap.getDepthMap(), depthMatrix);
    }

    //Render texture to quad.
    {
        glViewport(0, 0, GameInfo::getWindowWidth(), GameInfo::getWindowHeight());
        //don't override getProgramID when it's called.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.useProgram();
        screenQuad.render2D(screenShader, renderTexture.getTextureID());
    }
}

void RenderingSystem::renderAll(Camera& currentCamera, Time& currentTime, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap) {

    renderDebugging(currentCamera);
    renderModels(currentCamera, currentTime, pointLightDepthMap, directionalLightDepthMap);
    renderOthers(currentCamera, currentTime);
}

void RenderingSystem::renderDebugging(Camera& currentCamera) {
    // Execute debug drawing if enabled
    if (Shader::getShaderTask() != SHADER_TASK::Normal_Render_Task) {
        return;
    }

    const std::vector<DebuggingController*>& debugControllers = currentScene->getAllComponentsOfType<DebuggingController>();

    for (unsigned int i = 0; i < debugControllers.size(); i++) {
        debugControllers[i]->executeDebugRendering(*physicsWorld, *currentCamera.getViewMatrix(), *currentCamera.getProjectionMatrix());
    }

    systems->dayNightCycleSystem.debugRender(*currentCamera.getViewMatrix(), *currentCamera.getProjectionMatrix());
}

void RenderingSystem::renderModels(Camera& currentCamera, Time& currentTime, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap& directionalLightDepthMap) {

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
                supplyLitShaderUniforms(*shdr, pointLightDepthMap, directionalLightDepthMap, currentCamera, currentTime);
                glUniform1i(Shaders::getUniformLocation(shdr->getProgramID(), Shaders::UniformName::IsModelAnimated), isAnimated);

                modelToRender->renderAll(*shdr);
            }

            if (shdr->getShaderType() == SHADER_TYPE::Default) {

                shdr->useProgram();
                supplyDefaultShaderUniforms(*shdr, currentCamera, currentTime);

                glUniform1i(Shaders::getUniformLocation(shdr->getProgramID(), Shaders::UniformName::IsModelAnimated), isAnimated);

                modelToRender->renderAll(*shdr);
            }
        }

        return false;
    });
}

// Render Particles and GUI
void RenderingSystem::renderOthers(Camera& currentCamera, Time& currentTime) {

    //Particles and UI aren't 3D so depth render tasks don't need their info.
    if (Shader::getShaderTask() != SHADER_TASK::Normal_Render_Task) {
        return;
    }

    const std::vector<Particles*>& particles = currentScene->getAllComponentsOfType<Particles>();

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
        supplyDefaultShaderUniforms(*shdr, currentCamera, currentTime);
        systems->skyBoxSystem.render(*skyBox, currentCamera, *shdr);

        return false;
    });

    for (unsigned int i = 0; i < particles.size(); i++) {
        renderParticles(*particles[i], currentCamera, currentTime);
    }

    currentScene->loopEntities([&](const Scene::Entity& entity) {
        Shader* shader = currentScene->getComponent<Shader>(entity.id);

        if (!shader || shader->getShaderType() != SHADER_TYPE::GUI) {
            return false;
        }

        if (DisplayStatistics* stats = currentScene->getComponent<DisplayStatistics>(entity.id)) {
            shader->useProgram();
            systems->displayStatisticsSystem.render(*shader, *stats);
        }

        if (PauseMenu* menu = currentScene->getComponent<PauseMenu>(entity.id)) {
            shader->useProgram();
            systems->pauseMenuSystem.render(*shader, *menu);
        }

        return false;
    });
}

void RenderingSystem::renderParticles(Particles& particles, Camera& currentCamera, Time& currentTime) {
    if (Shader* thisShader = currentScene->getComponent<Shader>(particles.getEntityID())) {
        // Check to see if the shader is a particle shader
        if (thisShader->getShaderType() == SHADER_TYPE::Particle) {
            supplyParticleShaderUniforms(*thisShader, currentCamera, currentTime);

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

Shader* RenderingSystem::prepareShader(const int32_t& entity,
                                       PointLightShadowMap& pointLightDepthMap,
                                       DirectionalLightShadowMap& directionalLightDepthMap,
                                       Camera& currentCamera,
                                       Time& currentTime) {
    Shader* thisShader;

    if (thisShader = currentScene->getComponent<Shader>(entity)) {

        if (thisShader->getShaderType() == SHADER_TYPE::Default) {
            if (currentScene->isEntityActive(entity)) {
                thisShader->useProgram();
                supplyDefaultShaderUniforms(*thisShader, currentCamera, currentTime);
                return thisShader;
            }
        }

        if (thisShader->getShaderType() == SHADER_TYPE::Lit) {
            if (currentScene->isEntityActive(entity)) {
                thisShader->useProgram();
                supplyLitShaderUniforms(*thisShader,
                                        pointLightDepthMap,
                                        directionalLightDepthMap,
                                        currentCamera,
                                        currentTime);
                return thisShader;
            }
        }
    }

    return nullptr;
}

void RenderingSystem::supplyDefaultShaderUniforms(Shader& shader, Camera& currentCamera, Time& currentTime) {

    const GLint& programID = shader.getProgramID();

    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ViewMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getViewMatrix()));
    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ProjectionMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getProjectionMatrix()));

    glUniform3f(Shaders::getUniformLocation(programID, Shaders::UniformName::ViewPosition), currentCamera.position.x, currentCamera.position.y, currentCamera.position.z);
}

void RenderingSystem::supplyLitShaderUniforms(Shader& shader,
                                              PointLightShadowMap& pointLightDepthMap,
                                              DirectionalLightShadowMap& directionalLightDepthMap,
                                              Camera& currentCamera,
                                              Time& currentTime) {

    supplyDefaultShaderUniforms(shader, currentCamera, currentTime);

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

void RenderingSystem::supplyParticleShaderUniforms(Shader& particleShader, Camera& currentCamera, Time& currentTime) {
    particleShader.useProgram();

    const GLint& programID = particleShader.getProgramID();

    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ViewMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getViewMatrix()));
    glUniformMatrix4fv(Shaders::getUniformLocation(programID, Shaders::UniformName::ProjectionMatrix), 1, GL_FALSE, glm::value_ptr(*currentCamera.getProjectionMatrix()));
}
