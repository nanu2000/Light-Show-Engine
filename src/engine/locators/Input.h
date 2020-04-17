#ifndef _INPUT_H
#define _INPUT_H
#include "GameInfo.h"
#include "glm/vec2.hpp"
#include <SDL.h>
#include <algorithm>
#include <vector>

//!Mouse buttons
enum class MOUSE_BUTTON : int8_t {
    LeftButton,
    RightButton,
    MiddleButton
};

namespace Engine {
    namespace InputData {
        //!Time it takes to trigger another press for isPressedOnce
        extern float TIME_BETWEEN_KEY_PRESS_RESET;

        //!Time it takes to trigger contious keypress for isPressedOnce
        extern float TIME_UNTIL_CONTINIOUS_PRESS_RESET;

        //!Disable continious press
        extern bool NO_CONTINIOUS_PRESS;
    }
}

class Input {

public:
    Input();

    //!Called in Application.cpp before sdl events are supplied via handleEvent.
    virtual void preEvents();

    //!Called in Application.cpp after sdl events are supplied via handleEvent.
    virtual void postEvents(float dt);

    //!Checks if the mouse is pressed and returns true if the timeBetweenPress is zero, if it's the first press, or if timeUntilContinious is 0.
    //!NOTE: isPressedOnce function's timeBeetweenPress only works properly in Update, not FixedUpdate.
    virtual bool isMousePressedOnce(MOUSE_BUTTON);

    //!Checks if a mouse button is pressed.
    virtual bool isMouseButtonPressed(MOUSE_BUTTON);

    //!Checks if the key is pressed and returns true if the timeBetweenPress is zero, if it's the first press, or if timeUntilContinious is 0.
    //!NOTE: isPressedOnce function's timeBeetweenPress only works properly in Update, not FixedUpdate.
    virtual bool isKeyPressedOnce(const SDL_Keycode& keycode);

    //!Checks if a key is pressed.
    virtual bool isKeyDown(const SDL_Keycode& keycode);

    //!Called by Application.cpp to handle SDL's new events.
    virtual void handleEvent(SDL_Event& sdlEventSystem, SDL_EventType t);

    //!Returns the mouse position relative to the top left of the screen.
    inline virtual glm::ivec2 getMousePosition() const {
        return mousePosition;
    }

    //!Returns the mouse position relative to the center of the screen.
    //!To get the delta value, the mouse position is expected to be set to the window's center every FixedUpdate.
    inline virtual glm::ivec2 getMouseDelta() const {
        return mouseDelta;
    }

private:
    //!Contains data for all of the keys.
    struct KeyData {
        float timeUntilContinious = 0.f;
        float timeBetweenPress    = 0.f;

        bool isPressed = false;
        bool isKeyUp   = false; //for future.
    };

    //!For updating the timers every update. Gets cleared every update
    std::vector<KeyData*> currentPressedOnce;

    //!Checks if the KeyData d is pressed and returns true if the timeBetweenPress is zero, if it's the first press, or if timeUntilContinious is 0.
    //!NOTE: isPressedOnce function's timeBeetweenPress only works properly in Update, not FixedUpdate.
    bool isPressedOnce(KeyData& data);

    KeyData keyData[SDL_NUM_SCANCODES];
    KeyData mouseData[3];

    glm::ivec2 mousePosition = glm::ivec2(0, 0);
    glm::ivec2 mouseDelta    = glm::ivec2(0, 0);
};

//!Used for the InputLocator if no services have been provided
class NullInput : public Input {

public:
    NullInput();

    void handleEvent(SDL_Event& sdlEventSystem, SDL_EventType t) override;

    bool isKeyDown(const SDL_Keycode& keycode) override;

    bool isMouseButtonPressed(MOUSE_BUTTON) override;

    bool isKeyPressedOnce(const SDL_Keycode& keycode) override;

    bool isMousePressedOnce(MOUSE_BUTTON) override;

    virtual void postEvents(float dt) override {}

    inline virtual glm::ivec2 getMouseDelta() const override {
        return glm::ivec2(0, 0);
    }
    inline virtual glm::ivec2 getMousePosition() const override {
        return glm::ivec2(0, 0);
    }
};

#endif
