#ifndef USER_CONTROLS_H
#define USER_CONTROLS_H
#include "Component.h"
#include <SDL_keycode.h>

class UserControls : public Component<UserControls> {

public:
    SDL_Keycode getPlayerForwardKey() const { return playerControls.forwardKey; }
    SDL_Keycode getPlayerBackwardsKey() const { return playerControls.backwardsKey; }
    SDL_Keycode getPlayerLeftKey() const { return playerControls.leftKey; }
    SDL_Keycode getPlayerRightKey() const { return playerControls.rightKey; }
    SDL_Keycode getPlayerJumpKey() const { return playerControls.jumpKey; }
    SDL_Keycode getPlayerViewKey() const { return playerControls.viewKey; }
    SDL_Keycode getGuiControlPauseMenuKey() const { return guiControls.openPauseMenu; }

private:
    // struct CameraControls{}; add later

    struct PlayerControls {
        SDL_Keycode forwardKey   = SDLK_w;
        SDL_Keycode backwardsKey = SDLK_s;
        SDL_Keycode leftKey      = SDLK_a;
        SDL_Keycode rightKey     = SDLK_d;
        SDL_Keycode jumpKey      = SDLK_SPACE;
        SDL_Keycode viewKey      = SDLK_F3;
    };

    struct GuiControls {
        SDL_Keycode openPauseMenu = SDLK_ESCAPE;
    };

    GuiControls guiControls;

    PlayerControls playerControls;
};

#endif
