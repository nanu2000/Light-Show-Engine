#ifndef GUI_BASE
#define GUI_BASE

#include "Shader.h"
#include <stdint.h>

class GuiBase {

public:
    virtual void render(Shader& shader) = 0;
};

#endif
