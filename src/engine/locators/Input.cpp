#include "Input.h"

bool Input::isMousePressedOnce(MOUSE_BUTTON ind, float dt) {
    return isPressedOnce(mouseData[(int8_t)ind], dt);
}

bool Input::isKeyDown(const SDL_Keycode& keycode) {
    return keyData[SDL_GetScancodeFromKey(keycode)].isPressed;
}

bool Input::isKeyPressedOnce(const SDL_Keycode& keycode, float dt) {
    return isPressedOnce(keyData[SDL_GetScancodeFromKey(keycode)], dt);
}

Input::Input() {
    SDL_PumpEvents();
}

void Input::handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t) {

    switch (t) {
    case SDL_KEYDOWN:
        keyData[sdlEventSystem.key.keysym.scancode].isPressed = true;
        break;
    case SDL_KEYUP:
        keyData[sdlEventSystem.key.keysym.scancode].isPressed = false;
        break;
    case SDL_QUIT:
        //Call to close the game.
        GameInfo::terminateGame();
        break;
    case SDL_MOUSEBUTTONDOWN:
        mouseData[sdlEventSystem.button.button - 1].isPressed = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mouseData[sdlEventSystem.button.button - 1].isPressed = false;
        break;
    default:
        break;
    }

    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
}

bool Input::isPressedOnce(KeyData& data, float dt) {

    if (data.isPressed) {
        if (data.timeUntilContinious == 0 && data.timeBetweenPress == 0) {
            //Set to reset value and return true
            data.timeUntilContinious = data.timeUntilContiniousReset;
            data.timeBetweenPress    = data.timeBetweenPressReset;

            //Return true on first input
            return true;
        }

        data.timeUntilContinious -= dt;
        if (data.timeUntilContinious <= 0) {
            return true;
        }

        data.timeBetweenPress -= dt;
        if (data.timeBetweenPress <= 0) {
            data.timeBetweenPress = data.timeBetweenPressReset;
            return true;
        }

    } else {

        data.timeUntilContinious = 0;
        data.timeBetweenPress    = 0;
        return false;
    }

    return false;
}

bool Input::isMouseButtonPressed(MOUSE_BUTTON ind) {
    int8_t index = (int8_t)ind;

    if (index <= 2 && index >= 0) {
        return mouseData[index].isPressed;
    } else {
        return false;
    }
}
//Below is the Null input class for the service provider

NullInput::NullInput() {
}
bool NullInput::isKeyPressedOnce(const SDL_Keycode& keycode, float dt) {
    return false;
}
void NullInput::handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t) {
    if (t == SDL_QUIT) {
        /**********************|
		|We exit the game Here |
		|**********************/
        GameInfo::terminateGame();
        /*********************/
    }
}

bool NullInput::isMousePressedOnce(MOUSE_BUTTON index, float dt) {
    return false;
}

bool NullInput::isKeyDown(const SDL_Keycode& keycode) {
    return false;
}

bool NullInput::isMouseButtonPressed(MOUSE_BUTTON index) {
    return false;
}
