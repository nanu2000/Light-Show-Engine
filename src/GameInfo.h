#ifndef GAME_INFO_H
#define GAME_INFO_H
#include "LinearMath/btTransform.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <string>

//Todo: Move to json for entities in scene.
enum class COLLISION_TAGS : int16_t {
    Player           = 0,
    TestCollisionTag = 1,
    TAG_EMPTY        = 2,
    TAGS_MAX_SIZE    = 3
};
static const char* COLLISION_TAG_C_STR[static_cast<int16_t>(COLLISION_TAGS::TAGS_MAX_SIZE)] = {
    "Player",
    "Test",
    "Empty"
};

static const char* GET_COLLISION_TAG_NAME(const COLLISION_TAGS& cTag) {
#ifdef DEBUG
    if (cTag >= COLLISION_TAGS::TAGS_MAX_SIZE) {
        DBG_LOG("cTag Goes Out of COLLISION_TAG_C_STR Bounds! (static const char* GET_COLLISION_TAG_NAME in GameInfo.h)\n");
        return "";
    }
#endif

    return COLLISION_TAG_C_STR[static_cast<int16_t>(cTag)];
}

const int8_t TAG_ENTITY_UNDEFINED = -1;
//

//deltaTime for physics calculations and accurate updating
namespace PrivateGameInfo {
    extern float deltaTime;
};

//!Contains the game's info such as Window settings and timestep.
namespace GameInfo {

    /*For Screen Resolution Dependent Images / text, use this formula.
	**************************************************************************************
	
		width	=	SIZEW / GameInfo::START_WINDOW_WIDTH * GameInfo::getWindowWidth(),
		height	=	SIZEH / GameInfo::START_WINDOW_WIDTH * GameInfo::getWindowWidth()	

	**************************************************************************************/

    const glm::vec3 GLM_VEC3_ZERO = glm::vec3(0, 0, 0);
    const btVector3 BT_VEC3_ZERO  = btVector3(0, 0, 0);

    const unsigned int MAX_COLLISION_TRIGGERS_AT_ONCE = 5;

    //The Default Messenger Capacity.
    const unsigned short DEFAULT_MESSAGE_CAPACITY = 10;

    //fixedDeltaTime for physics calculations and accurate updating
    const float fixedDeltaTime = 1.f / 30.f;

    extern float getDeltaTime();

    //Max amount of substeps
    const short PHYSICS_MAX_SUBSTEPS = 1;

    //The physics timestep
    const float PHYSICS_TIME_STEP = fixedDeltaTime;

    //Default Window Title
    const std::string WINDOW_TITLE = "Gonna get there!";

    //For Debug Drawing
    const glm::vec4 DEBUG_COLOR = glm::vec4(0.0f, 0.7f, 0.3f, 1.0f);

    //The default gravity used for physics worlds
    const glm::vec3 DEFAULT_GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f);

    //Starting Window Width in pixels
    const unsigned short START_WINDOW_WIDTH = 800;

    //Starting Window Height in pixels
    const unsigned short START_WINDOW_HEIGHT = 600;

    //Minimum Window Height in pixels
    const unsigned short MIN_WINDOW_HEIGHT = 300;

    //Minimum Window Width in pixels
    const unsigned short MIN_WINDOW_WIDTH = 400;

    void terminateGame();
    int getWindowWidth();
    int getWindowHeight();

    //Used to set the mouse position
    void setMousePosition(int xPos, int yPos);
}

#endif
