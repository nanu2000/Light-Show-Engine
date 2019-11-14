#ifndef SHADERS_H
#define SHADERS_H
#include <GL/glew.h>
#include <string>
namespace Shaders {

const std::string MAX_LIGHT_TAG        = "/*MAL*/";
const std::string SHADOW_INTENSITY_TAG = "/*SI*/";
const std::string SHADOW_FILTERING_TAG = "/*SF*/";

enum class SHADER_TYPE {
    Default         = 0,
    Particle        = 1,
    GUI             = 2,
    SHADER_TYPE_MAX = 3
};

enum SHADER_TASK {
    Normal_Render_Task         = 0,
    Directional_Depth_Task     = 1,
    Omnidirectional_Depth_Task = 2,
    SHADER_TASK_MAX            = 3
};

//The end tag inside the shaders (#define xxxxx /*TAG*/value//)
const std::string TAG_END = "//";

//The location the depth map for omnidirectional shadows is going to be bound.
// use by adding to GL_TEXTURE_0 (GL_TEXTURE0 + DEPTH_MAP_LOCATION_OMNIDIRECTIONAL)
const unsigned short DEPTH_MAP_LOCATION_OMNIDIRECTIONAL = 6;

//The location the depth map for directional shadows is going to be bound.
// use by adding to GL_TEXTURE_0 (GL_TEXTURE0 + DEPTH_MAP_LOCATION_DIRECTIONAL)
const unsigned short DEPTH_MAP_LOCATION_DIRECTIONAL = 7;

enum class UniformName {

    DiffuseTexture  = 0,
    SpecularTexture = 1,
    NormalTexture   = 2,

    BoneWeights         = 3,
    BoneIDS             = 4,
    BoneTransformations = 5,

    TextureCoordinates = 6,
    Position           = 7,
    Normals            = 8,

    ModelMatrix      = 9,
    ViewMatrix       = 10,
    ProjectionMatrix = 11,

    ViewPosition     = 12,
    LightSpaceMatrix = 13,
    LightPosition    = 14,

    FarPlane             = 15,
    ShadowMatrices       = 16,
    DirectionalShadowMap = 17,
    PointShadowMap       = 18,
    IsModelAnimated      = 19,
    Color                = 20,
    ProjectionViewMatrix = 21,
    DepthMap             = 22,
    NearPlane            = 23,

    DirectionalLightDirection = 24,
    DirectionalLightAmbient   = 25,
    DirectionalLightDiffuse   = 26,
    DirectionalLightSpecular  = 27,
    MaterialDiffuse           = 28,
    MaterialAmbient           = 29,
    MaterialSpecular          = 30,
    MaterialShininess         = 31,
    TimeMS                    = 33,
    UNIFORM_NAME_COUNT        = 34

};

static const char* UniformNames[34] = {
    "material.texture_diffuse",
    "material.texture_specular",
    "material.texture_normals",
    "boneWeights",
    "boneIds",
    "boneTransformation",
    "textureCoords",
    "position",
    "normal",
    "model",
    "view",
    "projection",
    "viewPosition",
    "lightSpaceMatrix",
    "lightPosition",
    "farPlane",
    "shadowMatrices",
    "directionalShadowMap",
    "pointShadowMap",
    "isAnimated",
    "colorIn",
    "projectionView",
    "depthMap",
    "nearPlane",
    "directionalLight.direction",
    "directionalLight.ambient",
    "directionalLight.diffuse",
    "directionalLight.specular",
    "material.diffuse",
    "material.ambient",
    "material.specular",
    "material.shininess",
    "timeMS"
};

enum class AttribName {
    Position           = 0,
    Normal             = 1,
    TextureCoordinates = 2,
    BoneWeights        = 3,
    BoneIDS            = 4,

    ParticlePosition = 5,
    ParticleScale    = 6,
    ParticleColor    = 7,

    AttribName_MAX = 8,
};

static const char* getUniformName(const UniformName& name) {
    if (name < UniformName::UNIFORM_NAME_COUNT) {
        return UniformNames[static_cast<int32_t>(name)];
    }
    return "NA";
}

static GLint getAttribLocation(const AttribName& name) {
    return static_cast<GLint>(name);
}

static GLint getUniformLocation(GLint program, const UniformName& name) {
    return glGetUniformLocation(program, getUniformName(name));
}

}
#endif
