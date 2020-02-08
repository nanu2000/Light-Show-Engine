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

    virtual bool mousePressedOnce(MOUSE_BUTTON);

    virtual bool isKeyDown(const SDL_Keycode& keycode);

    virtual bool keyPressedOnce(const SDL_Keycode& keycode);

    virtual bool getMouseButton(MOUSE_BUTTON);

    virtual void handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t);

    virtual glm::ivec2 getMousePosition() const;
    virtual glm::ivec2 getMouseDelta() const;

private:
    bool keys[SDL_NUM_SCANCODES];

    glm::ivec2 mousePosition = glm::ivec2(0, 0);

    bool mouseArray[3] = { false, false, false };

    bool hitOnce[3] = { false, false, false };

    bool pressed[3] = { false, false, false };

    glm::ivec2 mouseLastPosition;
};

class NullInput : public Input {

public:
    NullInput();

    void handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t) override;

    bool isKeyDown(const SDL_Keycode& keycode) override;

    bool getMouseButton(MOUSE_BUTTON) override;

    bool keyPressedOnce(const SDL_Keycode& keycode) override;

    bool mousePressedOnce(MOUSE_BUTTON) override;

    virtual glm::ivec2 getMouseDelta() const override;

    glm::ivec2 getMousePosition() const override;
};

#endif
