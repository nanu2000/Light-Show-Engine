#ifndef GUI_BASE
#define GUI_BASE

#include <stdint.h>
#include "ShaderBase.h"

class GuiBase {

public:
    virtual void render(ShaderBase& shader) = 0;
};

#endif
