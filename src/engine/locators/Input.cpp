#include "Input.h"

bool Input::isMousePressedOnce(MOUSE_BUTTON ind) {

    int8_t index = (int8_t)ind;

    bool isPressed = isMouseButtonPressed(ind);

    return false;
}

bool Input::isKeyDown(const SDL_Keycode& keycode) {
    return keys[SDL_GetScancodeFromKey(keycode)];
}

bool Input::isKeyPressedOnce(const SDL_Keycode& keycode, float dt) {
    SDL_Scancode scancode              = SDL_GetScancodeFromKey(keycode);
    PressedOnceLogic& pressedOnceLogic = keysPressedOnce[scancode];
    bool isKeyDown                     = keys[scancode];

    if (isKeyDown) {
        if (pressedOnceLogic.timeUntilContinious == 0 && pressedOnceLogic.timeBetweenPress == 0) {
            //Set to reset value and return true
            pressedOnceLogic.timeUntilContinious = pressedOnceLogic.timeUntilContiniousReset;
            pressedOnceLogic.timeBetweenPress    = pressedOnceLogic.timeBetweenPressReset;

            //Return true on first input
            return true;
        }

        pressedOnceLogic.timeUntilContinious -= dt;
        if (pressedOnceLogic.timeUntilContinious <= 0) {
            return true;
        }

        pressedOnceLogic.timeBetweenPress -= dt;
        if (pressedOnceLogic.timeBetweenPress <= 0) {
            pressedOnceLogic.timeBetweenPress = pressedOnceLogic.timeBetweenPressReset;
            return true;
        }

    } else {

        pressedOnceLogic.timeUntilContinious = 0;
        pressedOnceLogic.timeBetweenPress    = 0;
        return false;
    }

    return false;
}

Input::Input() {
    SDL_PumpEvents();

    //set all keys as false
    std::fill_n(keys, SDL_NUM_SCANCODES, false);
}

void Input::handleEvents(SDL_Event& sdlEventSystem, SDL_EventType t) {
    if (t == SDL_KEYDOWN) {
        keys[sdlEventSystem.key.keysym.scancode] = true;
    }
    if (t == SDL_KEYUP) {
        keys[sdlEventSystem.key.keysym.scancode] = false;
    }

    if (t == SDL_QUIT) {
        /**********************|
		|We exit the game Here |
		|**********************/
        GameInfo::terminateGame();
        /*********************/
    }

    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    if (t == SDL_MOUSEBUTTONDOWN) {
        mouseArray[sdlEventSystem.button.button - 1] = true;
    }

    if (t == SDL_MOUSEBUTTONUP) {
        mouseArray[sdlEventSystem.button.button - 1] = false;
    }
}

bool Input::isMouseButtonPressed(MOUSE_BUTTON ind) {
    int8_t index = (int8_t)ind;

    if (index <= 2 && index >= 0) {
        return mouseArray[index];
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

bool NullInput::isMousePressedOnce(MOUSE_BUTTON index) {
    return false;
}

bool NullInput::isKeyDown(const SDL_Keycode& keycode) {
    return false;
}

bool NullInput::isMouseButtonPressed(MOUSE_BUTTON index) {
    return false;
}
