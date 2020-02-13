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

    virtual bool isMousePressedOnce(MOUSE_BUTTON);

    virtual bool isKeyDown(const SDL_Keycode& keycode);

    virtual bool isKeyPressedOnce(const SDL_Keycode& keycode, float dt);

    virtual bool isMouseButtonPressed(MOUSE_BUTTON);

    virtual void handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t);

    inline virtual glm::ivec2 Input::getMousePosition() const {
        return mousePosition;
    }

    inline virtual glm::ivec2 Input::getMouseDelta() const {
        return glm::vec2(mousePosition.x - GameInfo::getWindowWidth() / 2.f, mousePosition.y - GameInfo::getWindowHeight() / 2);
    }

private:
    struct PressedOnceLogic {
        float timeUntilContinious      = 0.f; //one second until press is continious.
        float timeUntilContiniousReset = 1.f;

        float timeBetweenPress      = 0.f;
        float timeBetweenPressReset = .33f;
    };

    bool keys[SDL_NUM_SCANCODES];
    PressedOnceLogic keysPressedOnce[SDL_NUM_SCANCODES];

    glm::ivec2 mousePosition     = glm::ivec2(0, 0);
    glm::ivec2 mouseLastPosition = glm::ivec2(0, 0);

    bool mouseArray[3] = { false, false, false };

    bool pressed[3] = { false, false, false };
};

class NullInput : public Input {

public:
    NullInput();

    void handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t) override;

    bool isKeyDown(const SDL_Keycode& keycode) override;

    bool isMouseButtonPressed(MOUSE_BUTTON) override;

    bool isKeyPressedOnce(const SDL_Keycode& keycode, float dt) override;

    bool isMousePressedOnce(MOUSE_BUTTON) override;

    inline virtual glm::ivec2 NullInput::getMouseDelta() const override {
        return glm::ivec2(0, 0);
    }
    inline virtual glm::ivec2 NullInput::getMousePosition() const override {
        return glm::ivec2(0, 0);
    }
};

#endif
