
#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include "GameInfo.h"
#include "Locator.h"
#include <LinearMath/btIDebugDraw.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace Engine {

    //!Information used for the DebugDrawer.
    namespace DBG_DRAWER {
        static const unsigned int MAX_AMOUNT_DEBUG_LINES = 16382;
        static const float DEBUG_LINE_WIDTH              = .75f;
    }

    //!The DebugDrawer class is used to render Bullet3's collision boxes/points/etc..
    //!The DebugDrawer should never be created outside of the PhysicsWorld object.
    class DebugDrawer : public btIDebugDraw {
    public:
        DebugDrawer() {}
        ~DebugDrawer();

        //!Sets the colors for debug rendering.
        virtual DefaultColors getDefaultColors() const {
            DefaultColors colors;

            colors.m_activeObject               = btVector3(.8, .8, .8);
            colors.m_disabledSimulationObject   = btVector3(.8, .8, 0);
            colors.m_contactPoint               = btVector3(.8, .8, 0);
            colors.m_disabledDeactivationObject = btVector3(.4, 0, 0);
            colors.m_aabb                       = btVector3(.4, 0, 0);
            colors.m_wantsDeactivationObject    = btVector3(0, .4, .4);
            colors.m_deactivatedObject          = btVector3(0, .4, 0);

            return colors;
        }

        //!Initializes everything
        void initialize();

        //!Renders the lines added via drawLine.
        void render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

        //!Adds a line to the list of lines to draw next frame
        virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

        //!Adds a line to the list of lines to draw next frame
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& colorStart, const btVector3& colorEnd);

        //!Unimplemented
        virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}

        virtual void reportErrorWarning(const char* warningString);

        //!Unimplemented
        virtual void draw3dText(const btVector3&, const char*) {}

        //!Sets the debug draw mode
        virtual void setDebugMode(int debugMode);

        //!Gets the debug draw mode
        virtual int getDebugMode() const { return currentDebugMode; }

    private:
        Shader thisShader;

        glm::mat4 identitym = glm::mat4(1.0f);

        glm::vec3 lineVertices[Engine::DBG_DRAWER::MAX_AMOUNT_DEBUG_LINES];
        glm::vec3 lineColors[Engine::DBG_DRAWER::MAX_AMOUNT_DEBUG_LINES];

        int currentDebugMode                    = 0;
        unsigned int currentMaxAmountOfVertices = 0;
        unsigned int currentAmountOfLines       = 0;

        GLuint VAO     = 0;
        GLuint vertID  = 0;
        GLuint colorID = 0;

        GLint positionAttribute  = 0;
        GLint colorAttribute     = 0;
        GLint projectionLocation = 0;
        GLint viewLocation       = 0;
        bool initialized         = false;
    };
}
#endif
