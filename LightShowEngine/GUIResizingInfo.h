#ifndef GUI_RESIZING_INFO
#define GUI_RESIZING_INFO
#include "GameInfo.h"

class GUIResizingInformation
{

public:

	int getTopScreenPosition() const { return  GameInfo::getWindowHeight(); }
	int getBottomScreenPosition() const { return -GameInfo::getWindowHeight(); }
	int getLeftScreenPosition() const { return -GameInfo::getWindowWidth(); }
	int getRightScreenPosition() const { return GameInfo::getWindowWidth(); }

	float getWidthUnit() const { return widthByWidth; }
	float getHeightUnit() const { return heightByHeight; }

	void updateInformation()
	{
		widthByWidth = (float)GameInfo::getWindowWidth() / (float)GameInfo::START_WINDOW_WIDTH;
		heightByHeight = (float)GameInfo::getWindowHeight() / (float)GameInfo::START_WINDOW_HEIGHT;
	}


private:

	float widthByWidth = 0;
	float heightByHeight = 0;

};

#endif