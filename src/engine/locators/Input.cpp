#include "Input.h"

//!If TIME_BETWEEN_KEY_PRESS_RESET is > TIME_UNTIL_CONTINIOUS_PRESS_RESET, there will be no repeating of key press until continious timer expires.
//!This is most likely desired since currently TIME_BETWEEN_KEY_PRESS_RESET only works with update and not fixedUpdate.
float Engine::InputData::TIME_BETWEEN_KEY_PRESS_RESET      = 2.f;
float Engine::InputData::TIME_UNTIL_CONTINIOUS_PRESS_RESET = 1.f;

bool Input::isMousePressedOnce(MOUSE_BUTTON ind) {
    return isPressedOnce(mouseData[(int8_t)ind]);
}

bool Input::isKeyDown(const SDL_Keycode& keycode) {
    return keyData[SDL_GetScancodeFromKey(keycode)].isPressed;
}

bool Input::isKeyPressedOnce(const SDL_Keycode& keycode) {
    return isPressedOnce(keyData[SDL_GetScancodeFromKey(keycode)]);
}

Input::Input() {
    SDL_PumpEvents();
}

void Input::updateTimers(float dt) {

    //Loop through all of the currently pressed once keydata pointers
    for (unsigned int i = 0; i < currentPressedOnce.size(); i++) {

        if (currentPressedOnce.at(i)->isPressed == false) {

            currentPressedOnce.at(i)->timeUntilContinious = 0;
            currentPressedOnce.at(i)->timeBetweenPress    = 0;
            currentPressedOnce.erase(currentPressedOnce.begin() + i);
            continue;
        }

        //If this is true, then it is the first press.
        if (currentPressedOnce.at(i)->timeUntilContinious == 0 && currentPressedOnce.at(i)->timeBetweenPress == 0) {
            //Set to reset value and return true.
            currentPressedOnce.at(i)->timeUntilContinious = Engine::InputData::TIME_UNTIL_CONTINIOUS_PRESS_RESET;
            currentPressedOnce.at(i)->timeBetweenPress    = Engine::InputData::TIME_BETWEEN_KEY_PRESS_RESET;
        }

        //The time between press has expired so we reset it.
        if (currentPressedOnce.at(i)->timeBetweenPress <= 0) {
            currentPressedOnce.at(i)->timeBetweenPress = Engine::InputData::TIME_BETWEEN_KEY_PRESS_RESET;
        }

        //Subtract dt from timers.
        currentPressedOnce.at(i)->timeUntilContinious -= dt;
        currentPressedOnce.at(i)->timeBetweenPress -= dt;
    }
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

bool Input::isPressedOnce(KeyData& data) {

    if (data.isPressed) {

        //If the keydata is not in the timer vector add it.
        if (std::find(currentPressedOnce.begin(), currentPressedOnce.end(), &data) == currentPressedOnce.end()) {
            currentPressedOnce.push_back(&data);
        }

        if (data.timeUntilContinious == 0 && data.timeBetweenPress == 0) {

            //Return true on first input
            return true;
        }

        if (data.timeUntilContinious <= 0) {
            //Key has been pressed longer than Engine::InputData::TIME_UNTIL_CONTINIOUS_PRESS_RESET, return true!
            return true;
        }

        if (data.timeBetweenPress <= 0) {
            //Time between press has hit zero!
            return true;
        }

    } else {

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
bool NullInput::isKeyPressedOnce(const SDL_Keycode& keycode) {
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
