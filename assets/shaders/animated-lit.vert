#version 330 core

const int MAX_BONES = 64;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 boneTransformation[MAX_BONES];
uniform mat4 lightSpaceMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;
layout(location = 3) in vec4 boneWeights;
layout(location = 4) in vec4 boneIds;

out vec3 fragPosition_o;
out vec3 position_o;
out vec3 normal_o;
out vec2 textureCoords_o;
out vec4 fragmentPositionLightSpace_o;

void main() {

    //Transform the vertex information based on bones.
    //We use vec4(position, 1.0) because boneTransformation is a mat4, and you can't multiply a vec3 with a mat4.
    vec3 pos =  (boneWeights.x * (boneTransformation[int(boneIds.x)] * vec4(position, 1.0)).xyz) + 
                (boneWeights.y * (boneTransformation[int(boneIds.y)] * vec4(position, 1.0)).xyz) + 
                (boneWeights.z * (boneTransformation[int(boneIds.z)] * vec4(position, 1.0)).xyz) + 
                (boneWeights.w * (boneTransformation[int(boneIds.w)] * vec4(position, 1.0)).xyz);

    //Calculate the normals based on bones
    vec3 norm = (boneWeights.x * (boneTransformation[int(boneIds.x)] * vec4(normal, 0.0)).xyz) + 
                (boneWeights.y * (boneTransformation[int(boneIds.y)] * vec4(normal, 0.0)).xyz) + 
                (boneWeights.z * (boneTransformation[int(boneIds.z)] * vec4(normal, 0.0)).xyz) + 
                (boneWeights.w * (boneTransformation[int(boneIds.w)] * vec4(normal, 0.0)).xyz);

    //Supply outputs
    position_o                   = (view * model * vec4(pos, 1.0)).xyz;
    normal_o                     = mat3(transpose(inverse(model))) * norm;
    textureCoords_o              = textureCoords;
    fragPosition_o               = vec3(model * vec4(pos, 1.0f));
    fragmentPositionLightSpace_o = lightSpaceMatrix * vec4(fragPosition_o, 1.0);
   
    gl_Position = projection * view * model * vec4(pos, 1.0f);
}
