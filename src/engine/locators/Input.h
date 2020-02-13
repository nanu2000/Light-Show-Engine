#ifndef _INPUT_H
#define _INPUT_H
#include "GameInfo.h"
#include "glm/vec2.hpp"
#include <SDL.h>

enum class MOUSE_BUTTON : int8_t {
    LeftButton,
    RightButton,
    MiddleButton
};

class Input {

public:
    Input();

    //!Checks if the mouse is pressed and returns true if the timeBetweenPress is zero, if it's the first press, or if timeUntilContinious is 0.
    virtual bool isMousePressedOnce(MOUSE_BUTTON, float dt);
    //!Checks if a mouse button is pressed.
    virtual bool isMouseButtonPressed(MOUSE_BUTTON);

    //!Checks if the key is pressed and returns true if the timeBetweenPress is zero, if it's the first press, or if timeUntilContinious is 0.
    virtual bool isKeyPressedOnce(const SDL_Keycode& keycode, float dt);

    //!Checks if a key is pressed.
    virtual bool isKeyDown(const SDL_Keycode& keycode);

    //!Called by Application.cpp to handle SDL's new events.
    virtual void handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t);

    //!Returns the mouse position relative to the top left of the screen.
    inline virtual glm::ivec2 Input::getMousePosition() const {
        return mousePosition;
    }

    //!Returns the mouse position relative to the center of the screen.
    //!To get the delta value, the mouse position is expected to be set to the window's center every FixedUpdate.
    inline virtual glm::ivec2 Input::getMouseDelta() const {
        return glm::vec2(mousePosition.x - GameInfo::getWindowWidth() / 2.f, mousePosition.y - GameInfo::getWindowHeight() / 2);
    }

private:
    //!Contains data for all of the keys.
    struct KeyData {
        float timeUntilContinious      = 0.f; //one second until press is continious.
        float timeUntilContiniousReset = 1.f;

        float timeBetweenPress      = 0.f; //time between returning true if pressed on isPressedOnce
        float timeBetweenPressReset = .33f;

        bool isPressed = false;
        bool isKeyUp   = false; //for future.
    };

    //!Checks if the KeyData d is pressed and returns true if the timeBetweenPress is zero, if it's the first press, or if timeUntilContinious is 0.
    bool isPressedOnce(KeyData& data, float dt);

    KeyData keyData[SDL_NUM_SCANCODES];
    KeyData mouseData[3];

    glm::ivec2 mousePosition     = glm::ivec2(0, 0);
    glm::ivec2 mouseLastPosition = glm::ivec2(0, 0);
};

//!Used for the InputLocator if no services have been provided
class NullInput : public Input {

public:
    NullInput();

    void handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t) override;

    bool isKeyDown(const SDL_Keycode& keycode) override;

    bool isMouseButtonPressed(MOUSE_BUTTON) override;

    bool isKeyPressedOnce(const SDL_Keycode& keycode, float dt) override;

    bool isMousePressedOnce(MOUSE_BUTTON, float dt) override;

    inline virtual glm::ivec2 NullInput::getMouseDelta() const override {
        return glm::ivec2(0, 0);
    }
    inline virtual glm::ivec2 NullInput::getMousePosition() const override {
        return glm::ivec2(0, 0);
    }
};

#endif
