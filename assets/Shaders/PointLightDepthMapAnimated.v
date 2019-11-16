#version 330 core
layout(location = 0) in vec3 position;
layout(location = 3) in vec4 boneWeights;
layout(location = 4) in vec4 boneIds;

const int MAX_BONES = 64;
uniform mat4 model;

uniform bool isAnimated;
uniform mat4 boneTransformation[MAX_BONES];

void main() {
    vec3 pos = position;

    //To get rid of this if statement, the vec4(position) will need to be multiplied by one so it does not equal zero.
    if (isAnimated) {
        pos = (boneWeights.x * (boneTransformation[int(boneIds.x)] * vec4(position, 1.0)).xyz) + (boneWeights.y * (boneTransformation[int(boneIds.y)] * vec4(position, 1.0)).xyz) + (boneWeights.z * (boneTransformation[int(boneIds.z)] * vec4(position, 1.0)).xyz) + (boneWeights.w * (boneTransformation[int(boneIds.w)] * vec4(position, 1.0)).xyz);
    }

    gl_Position = model * vec4(pos, 1.0);
}
