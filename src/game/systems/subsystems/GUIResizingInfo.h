#ifndef GUI_RESIZING_INFO
#define GUI_RESIZING_INFO
#include "GameInfo.h"
#include "SystemBase.h"

class GUIResizingInformation : public SystemBase {

public:
    virtual void recieveMessage(const BackEndMessages& msg, Scene& currentScene) override {

        if (msg == BackEndMessages::REFRESH_CAMERA) {
            updateInformation();
        }
    }

    int getTopScreenPosition() const { return GameInfo::getWindowHeight(); }
    int getBottomScreenPosition() const { return -GameInfo::getWindowHeight(); }
    int getLeftScreenPosition() const { return -GameInfo::getWindowWidth(); }
    int getRightScreenPosition() const { return GameInfo::getWindowWidth(); }

    float getWidthUnit() const { return widthByWidth; }
    float getHeightUnit() const { return heightByHeight; }

private:
    void updateInformation() {
        widthByWidth   = (float)GameInfo::getWindowWidth() / (float)GameInfo::START_WINDOW_WIDTH;
        heightByHeight = (float)GameInfo::getWindowHeight() / (float)GameInfo::START_WINDOW_HEIGHT;
    }

    float widthByWidth   = 0;
    float heightByHeight = 0;
};

#endif
