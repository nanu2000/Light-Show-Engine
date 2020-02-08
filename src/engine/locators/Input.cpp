#include "Input.h"

bool Input::mousePressedOnce(MOUSE_BUTTON ind) {

    int8_t index = (int8_t)ind;

    hitOnce[index] = getMouseButton(ind) ? hitOnce[index] : false;

    pressed[index] = false;

    if (hitOnce[index] == false) {
        if (getMouseButton(ind)) {
            hitOnce[index] = true;
            return true;
        }
    }

    return false;
}

bool Input::isKeyDown(const SDL_Keycode& keycode) {
    return keys[SDL_GetScancodeFromKey(keycode)];
}

bool Input::keyPressedOnce(const SDL_Keycode& keycode) {
    const SDL_Scancode& thisCode = SDL_GetScancodeFromKey(keycode);
    if (keys[thisCode]) {
        keys[thisCode] = false;
        return true;
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

glm::ivec2 Input::getMouseDelta() const {
    return glm::vec2(mousePosition.x - GameInfo::getWindowWidth() / 2.f, mousePosition.y - GameInfo::getWindowHeight() / 2);
}

bool Input::getMouseButton(MOUSE_BUTTON ind) {
    int8_t index = (int8_t)ind;

    if (index <= 2 && index >= 0) {
        return mouseArray[index];
    } else {
        return false;
    }
}

glm::ivec2 Input::getMousePosition() const {
    return mousePosition;
}

//Below is the Null input class for the service provider

NullInput::NullInput() {
}
bool NullInput::keyPressedOnce(const SDL_Keycode& keycode) {
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

bool NullInput::mousePressedOnce(MOUSE_BUTTON index) {
    return false;
}

glm::ivec2 NullInput::getMouseDelta() const {
    return glm::ivec2(0, 0);
}

bool NullInput::isKeyDown(const SDL_Keycode& keycode) {
    return false;
}

bool NullInput::getMouseButton(MOUSE_BUTTON index) {
    return false;
}
glm::ivec2 NullInput::getMousePosition() const {
    return glm::ivec2(0, 0);
}
