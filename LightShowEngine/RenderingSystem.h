#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H

#include "LitShader.h"
#include "Model.h"
#include "Scene.h"
#include "PointLightShadowMap.h"
#include "DirectionalLightShadowMap.h"
#include "DebuggingController.h"
#include "FountainParticleEmitter.h"
#include "SystemBase.h"
#include "ThirdPersonCamera.h"
#include "GuiString.h"
#include "GuiSprite.h"
#include "GuiButton.h"


using _3DM::AnimatedModel;
using _3DM::Model;

class RenderingSystem : public SystemBase
{

public:

	void initialize		(Scene& scene, Settings& settings, PhysicsWorld& world, SubSystems& systems) override;
	void render			(PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap & directionalLightDepthMap, Time& currentTime);

private:


	struct RenderModel
	{
		ShaderBase * shader;
		ModelBase * model;
		bool isLitShader = false;
	};

	//This function searches for any particle systems that may exist.
	//As of 4/17/2016, there is only one particle system implemented, so it will only return either a nullptr,
	//or a fountain particle system pointer
	ParticleEmitter * getParticleEmitter(int32_t entity);

	void renderParticles(Particles& particles, Camera & currentCamera, Time & currentTime);
	
	void initializeLights(LitShader & litShader);
	void initializeModels(ShaderBase & shader, const int32_t& entity);


	//Prepares and retrieves the first shader found in scene that is associated with entity.
	//**Does use program.
	ShaderBase * prepareShader(const int32_t & entity, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap & directionalLightDepthMap, Camera & currentCamera, Time & currentTime);

	void renderSingleModel(ShaderBase& shader, ModelBase & model)
	{
		glUniform1i
		(
			Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::IsModelAnimated),
			model.isAnimatedModel()
		);
		model.renderAll(shader);	
	}
		
	void supplyLitShaderUniforms		(ShaderBase & shader, PointLightShadowMap& pointLightDepthMap, DirectionalLightShadowMap & directionalLightDepthMap, Camera & currentCamera, Time & currentTime);

	void supplyDefaultShaderUniforms	(ShaderBase& shader, Camera & currentCamera, Time & currentTime);

	void supplyParticleShaderUniforms	(ShaderBase& particleShader, Camera & currentCamera, Time & currentTime);

	void addModelToProperRenderVector(ModelBase& model, const int32_t& entity, std::vector<RenderModel>& transparent, std::vector<RenderModel>& notTransparent);

	void renderTransparentModel(ShaderBase& shader, ModelBase & model)
	{
	
	
	}

};
#endif
