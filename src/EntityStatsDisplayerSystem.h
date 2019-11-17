#ifndef ENTITY_STATS_DISPLAYER_SYSTEM_H
#define ENTITY_STATS_DISPLAYER_SYSTEM_H

#include "EntityStatsDisplayer.h"

class EntityStatsDisplayerSystem {

public:
    void fixedUpdate(EntityStatsDisplayer& displayer, const EntityStats& stats, GUIResizingInformation& guiInfo) {
        float unit = guiInfo.getWidthUnit();

        if (displayer.lastUnit != unit) {
            displayer.guiString.setPosition(glm::ivec2(guiInfo.getLeftScreenPosition() + unit * 64.0f, guiInfo.getTopScreenPosition() - unit * 128.0f));

            displayer.guiString.setScale(glm::fvec2(unit, unit));

            displayer.lastUnit = unit;
        }

        displayer.guiString.setString(
            "Health  : " + std::to_string(stats.health) + "\n"
                                                          "Stamina : "
            + std::to_string(stats.stamina));
    }

    void render(EntityStatsDisplayer& displayer, ShaderBase& shader) {
        displayer.guiString.render(shader);
    }
};

#endif
