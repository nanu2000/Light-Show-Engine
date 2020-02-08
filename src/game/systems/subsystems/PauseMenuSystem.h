#ifndef PAUSE_MENU_SYSTEM_H
#define PAUSE_MENU_SYSTEM_H

#include "GUIResizingInfo.h"
#include "PauseMenu.h"
#include "UserControls.h"

class PauseMenuSystem {
public:
    void update(Input& input, PauseMenu& menu, GUIResizingInformation& guiInfo, const UserControls& userControls) {

        float unit = guiInfo.getWidthUnit();

        if (menu.lastUnit != unit) {
            menu.resumeButton.setScale(glm::fvec2(unit * 128, unit * 64));

            menu.lastUnit = unit;

            menu.str.setScale(glm::vec2(unit, unit));
        }

        if (input.keyPressedOnce(userControls.getGuiControlPauseMenuKey())) {
            menu.isShowing = !menu.isShowing;
        }

        menu.resumeButton.update(input);

        if (menu.isShowing) {

            menu.resumeButton.onButtonClickedOnce(
                [& isShowing = menu.isShowing](MOUSE_BUTTON buttonPressed) {
                    if (buttonPressed == MOUSE_BUTTON::LeftButton) {
                        isShowing = false;
                    }
                });
        }
    }

    void render(Shader& shader, PauseMenu& menu) {
        if (menu.isShowing) {
            menu.str.setVerticalPadding(-16);
            menu.str.setPosition(glm::vec2(GameInfo::getWindowWidth() - menu.str.getWidthOfString() * 2, GameInfo::getWindowHeight()));
            menu.resumeButton.render(shader);
            menu.str.render(shader, "Paused\n:D");
        }
    }
};

#endif
