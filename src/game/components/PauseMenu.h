#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Component.h"
#include "GuiButton.h"
#include "GuiString.h"

class PauseMenu : public Component<PauseMenu> {

public:
    bool showing() const {
        return isShowing;
    }

    void initialize(TextMap& map, Texture& textImage) {
        str.initialize(map, textImage);
        resumeButton.initialize(TextureLocator::getService().getTexture("assets/Images/Accept.png", GL_NEAREST));
    }

private:
    GuiString str = GuiString(10);
    GuiButton resumeButton;
    bool isShowing = false;
    float lastUnit = 0.0f;

    friend class PauseMenuSystem;
};

#endif
