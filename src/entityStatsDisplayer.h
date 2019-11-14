#ifndef ENTITY_STATS_DISPLAYER
#define ENTITY_STATS_DISPLAYER
#include "Component.h"
#include "EntityStats.h"
#include "GUIResizingInfo.h"
#include "GuiString.h"

class EntityStatsDisplayer : public Component<EntityStatsDisplayer> {
public:
    void initialize(TextMap& textMap, Texture& texture) {
        guiString.initialize(textMap, texture);
    }

private:
    GuiString guiString = GuiString(21);

    float lastUnit = 0;
    friend class EntityStatsDisplayerSystem;
};

#endif
