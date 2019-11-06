#ifndef DISPLAY_STATISTICS_H
#define DISPLAY_STATISTICS_H
#include "Component.h"
#include "GUIResizingInfo.h"
#include "GuiString.h"
#include "Time.h"

class DisplayStatistics : public Component<DisplayStatistics> {

public:
    void initialize(TextMap& textMap, Texture& texture) {
        guiString.initialize(textMap, texture);
    }

private:
    GuiString guiString = GuiString(3);

    float lastUnit = 0;

    friend class DisplayStatisticsSystem;
};

#endif
