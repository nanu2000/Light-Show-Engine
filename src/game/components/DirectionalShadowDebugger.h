#ifndef DIRECTIONAL_SHADOW_DEBUGGER_H
#define DIRECTIONAL_SHADOW_DEBUGGER_H
#include "Component.h"
#include "Quad.h"

//!Component to render a depthmap to a quad
class DirectionalShadowDebugger : public Component<DirectionalShadowDebugger> {

public:
    void initialize(const glm::mat4& QuadTransform) {
        quadTransform = QuadTransform;
        depthShader   = ShaderLocator::getService().getShader("depthtest", "assets/shaders/depth.vert", "assets/shaders/depth.frag", SHADER_TYPE::GUI);
    }

private:
    glm::mat4 quadTransform = glm::mat4(1.0f);
    Shader depthShader;
    Quad depthQuad;

    //friend system for this component.
    friend class DirectionalShadowDebuggerSystem;
};

#endif
