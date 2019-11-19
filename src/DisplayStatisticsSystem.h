
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

        ds.currentInterval += GameInfo::fixedDeltaTime;
        ds.fpsAVG.push_back(time.getFPS());

        if (ds.currentInterval > ds.updateInterval) {

            int average = std::accumulate(ds.fpsAVG.begin(), ds.fpsAVG.end(), 0) / ds.fpsAVG.size();

            ds.currentFPS = "\nFPS: " + std::to_string(average);
            ds.fpsAVG.resize(0);
            ds.currentInterval = 0;
        }

        double mspf     = floor(100 * time.getMSPF()) / 100;
        std::string str = std::to_string(mspf);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);

        ds.currentMSPF = "MSPF: " + str;

        ds.guiString.setString(ds.currentMSPF + ds.currentFPS);
    }

    void render(ShaderBase& shader, DisplayStatistics& ds) {
        ds.guiString.render(shader);
    }
};

#endif
