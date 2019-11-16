#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Camera.h"

class FirstPersonCamera : public Component<FirstPersonCamera>, public Camera {
public:
    ~FirstPersonCamera() {}

private:
    GLfloat sensitivity = GameInfo::deltaTime * 5;
    friend class FirstPersonCameraSystem;
};

#endif
