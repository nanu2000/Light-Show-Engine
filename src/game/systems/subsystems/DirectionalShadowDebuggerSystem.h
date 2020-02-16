#ifndef DIRECTIONAL_SHADOW_DEBUGGER_SYSTEM_H
#define DIRECTIONAL_SHADOW_DEBUGGER_SYSTEM_H
#include "DirectionalLightShadowMap.h"
#include "DirectionalShadowDebugger.h"

//!DirectionalShadowDebugger's system
class DirectionalShadowDebuggerSystem {
public:
    void render(DirectionalShadowDebugger& dbgr, const DirectionalLightShadowMap& shadowMap) {

        dbgr.depthShader.useProgram();
        dbgr.depthQuad.render3D(dbgr.depthShader, shadowMap.getDepthMap(), dbgr.quadTransform);
    }
};

#endif
