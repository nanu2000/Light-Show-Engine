
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

        double mspf     = floor(100 * time.getMSPF()) / 100;
        std::string str = std::to_string(mspf);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);

        ds.currentMSPF = "MSPF: " + str;

        if (ds.lastFPS != time.getFPS()) {
            ds.lastFPS    = time.getFPS();
            ds.currentFPS = "\nFPS: " + std::to_string(ds.lastFPS);
        }

        ds.guiString.setString(ds.currentMSPF + ds.currentFPS);
    }

    void render(Shader& shader, DisplayStatistics& ds) {
        ds.guiString.render(shader);
    }
};

#endif
