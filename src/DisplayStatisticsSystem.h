
#ifndef DISPLAY_STATISTICS_SYSTEM_H
#define DISPLAY_STATISTICS_SYSTEM_H

#include "DisplayStatistics.h"

class DisplayStatisticsSystem {

public:
    void fixedUpdate(DisplayStatistics& ds, const Time& time, GUIResizingInformation& guiInfo) {

        float unit = guiInfo.getWidthUnit();

        if (ds.lastUnit != unit) {
            ds.guiString.setPosition(glm::ivec2(guiInfo.getLeftScreenPosition() + unit * 64.0f, guiInfo.getTopScreenPosition() - unit * 64.0f));

            ds.guiString.setScale(glm::fvec2(unit, unit));

            ds.lastUnit = unit;
        }

        ds.guiString.setString(
            std::to_string(time.getMSPF()));
    }

    void render(ShaderBase& shader, DisplayStatistics& ds) {
        ds.guiString.render(shader);
    }
};

#endif
