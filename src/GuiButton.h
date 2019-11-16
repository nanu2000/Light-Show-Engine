#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "GuiSprite.h"
#include "Input.h"

enum class BUTTON_STATE {
    Hovering,
    Pressed,
    Released,
    FirstClick,
    No_Feedback
};

class GuiButton : public GuiSprite {

public:
    using GuiSprite::GuiSprite;

    void update(Input& input) {

        currentState = BUTTON_STATE::No_Feedback;

        if (
            mouseIsInsideButton(
                mouseCoordsToScreenCoords(
                    input.getMousePosition()))) {
            currentState = BUTTON_STATE::Hovering;

            if (input.getMouseButton(MOUSE_BUTTON::LeftButton)) {
                mouseButton  = MOUSE_BUTTON::LeftButton;
                currentState = BUTTON_STATE::Pressed;

                if (setFirstClick == false) {
                    currentState  = BUTTON_STATE::FirstClick;
                    setFirstClick = true;
                }

            } else if (input.getMouseButton(MOUSE_BUTTON::RightButton)) {
                mouseButton  = MOUSE_BUTTON::RightButton;
                currentState = BUTTON_STATE::Pressed;

                if (setFirstClick == false) {
                    currentState  = BUTTON_STATE::FirstClick;
                    setFirstClick = true;
                }

            } else if (setFirstClick) {
                setFirstClick = false;
                currentState  = BUTTON_STATE::Released;
            }
        }
    }

    template <typename Func>
    void onButtonHover(Func function) {
        if (currentState == BUTTON_STATE::Hovering) {
            function(mouseButton);
        }
    }

    template <typename Func>
    void onButtonClickedOnce(Func function) {
        if (currentState == BUTTON_STATE::FirstClick) {
            function(mouseButton);
        }
    }

    template <typename Func>
    void onButtonPressed(Func function) {
        if (currentState == BUTTON_STATE::Pressed) {
            function(mouseButton);
        }
    }

    template <typename Func>
    void onButtonReleased(Func function) {
        if (currentState == BUTTON_STATE::Released) {
            function(mouseButton);
        }
    }

    BUTTON_STATE getButtonState() const {
        return currentState;
    }

private:
    //The mouse coordinates origin is at the top left of the screen, but the images origin is in the center.
    glm::ivec2 mouseCoordsToScreenCoords(const glm::ivec2& mousePosition) {
        return glm::ivec2(
            2 * (mousePosition.x - GameInfo::getWindowWidth() / 2),
            2 * -(mousePosition.y - GameInfo::getWindowHeight() / 2));
    }

    bool mouseIsInsideButton(glm::ivec2 mousePosition) {

        return mousePosition.x <= position.x + (scale.x) && //x
            mousePosition.x >= position.x - (scale.x) && //x
            mousePosition.y >= position.y - (scale.y) && //y
            mousePosition.y <= position.y + scale.y; //y
    }

    bool setFirstClick = false;

    MOUSE_BUTTON mouseButton;
    BUTTON_STATE currentState = BUTTON_STATE::No_Feedback;
};

#endif
