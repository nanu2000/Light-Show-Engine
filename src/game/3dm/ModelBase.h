#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include "Transform.h"

class ModelBase {
public:
    virtual void renderSingleMesh(unsigned int index, Shader& shader) = 0;
    virtual void renderAll(Shader& shader)                            = 0;
    virtual void initialize(Shader& shader)                           = 0;

    virtual ~ModelBase() {}
    bool isAnimatedModel() { return animatedModel; }

    Transform transform;

protected:
    bool animatedModel = false;
};

#endif
