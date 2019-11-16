#ifndef SKY_BOX_H
#define SKY_BOX_H
#include "Component.h"
#include "Cube.h"

class SkyBox : public Component<SkyBox> {

public:
    CubeShape* getCube() { return &cube; }
    GLuint getCubeMap() { return map; }
    void supplyMap(GLuint m) { map = m; }

private:
    CubeShape cube;
    GLuint map;
};

#endif
