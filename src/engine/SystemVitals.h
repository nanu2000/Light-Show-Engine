#ifndef SYSTEM_VITALS_H
#define SYSTEM_VITALS_H
#include "DirectionalLightShadowMap.h"
#include "GameState.h"
#include "LTime.h"
#include "PhysicsWorld.h"
#include "PointLightShadowMap.h"
#include "RenderTexture.h"
#include "Shader.h"
#include "TextMap.h"
namespace Engine {

    //!The SystemVitals class is used to wrap important objects that need to be provided to the systems.
    //!It's lifecycle should be managed by the game.
    class SystemVitals {
    public:
        SystemVitals(Time& time, PhysicsWorld& w, GameState& gs) {
            currentTime  = &time;
            gameState    = &gs;
            physicsWorld = &w;
            isVitalsNull();
        }

        void resupply(Time& time, PhysicsWorld& w, GameState& gs) {
            currentTime  = &time;
            gameState    = &gs;
            physicsWorld = &w;
            isVitalsNull();
        }

        //!Initializes the text maps.
        void initializeTextMaps() {
            textMap.createMap("assets/fonts/courier-new.FontDat");
        }

        //!Sets shaders for depth map shader tasks and initializes depth maps and render textures.
        void initializeDepthMaps() {

            directionalLightDepthMap.initialize();
            pointLightDepthMap.initialize();
            renderTexture.initialize(GameInfo::getWindowWidth(), GameInfo::getWindowHeight());

            Shader::setShaderTaskShader(SHADER_TASK::Directional_Depth_Task, directionalLightDepthMap.getDepthMapShader());
            Shader::setShaderTaskShader(SHADER_TASK::Omnidirectional_Depth_Task, pointLightDepthMap.getDepthMapShader());
        }

        bool isVitalsNull() {
            if (!currentTime || !gameState || !physicsWorld) {
                DBG_LOG("ONE OR MORE VITAL OBJECTS IS NULL\n (Game::SystemVitals::isVitalsNull())\n");
                return true;
            }

            return false;
        }

        //We can always add more font atlases
        inline TextMap& getTextMap() { return textMap; }
        inline Settings& getSettings() { return settings; }
        inline PointLightShadowMap& getPointShadowMap() { return pointLightDepthMap; }
        inline DirectionalLightShadowMap& getDirectionalShadowMap() { return directionalLightDepthMap; }
        inline RenderTextureMS& getRenderTexture() { return renderTexture; }
        inline PhysicsWorld& getPhysicsWorld() { return *physicsWorld; }
        inline GameState& getGameState() { return *gameState; }
        inline Time& getTime() { return *currentTime; }

    private:
        TextMap textMap;
        Settings settings = Settings(GameInfo::DEFAULT_GRAVITY, glm::vec3(-20, 0, 10));
        PointLightShadowMap pointLightDepthMap;
        DirectionalLightShadowMap directionalLightDepthMap;
        RenderTextureMS renderTexture;
        PhysicsWorld* physicsWorld = nullptr;
        GameState* gameState       = nullptr;
        Time* currentTime          = nullptr;
    };
};
#endif
