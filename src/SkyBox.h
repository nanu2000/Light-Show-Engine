#ifndef SKY_BOX_H
#define SKY_BOX_H
#include "Component.h"
#include "Cube.h"

class SkyBox : public Component<SkyBox> {

public:
    CubeShape* getCube() { return &cube; }
    CubeMap* getCubeMap() {

        DBG_CHECK(map);

        return map;
    }
    void supplyMap(CubeMap& m) { map = &m; }

private:
    CubeShape cube;
    CubeMap* map;
};

#endif
