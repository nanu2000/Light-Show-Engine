#ifndef LIT_SHADER_H
#define LIT_SHADER_H
#include "Component.h"
#include "Lights.h"
#include "Material.h"
#include "Settings.h"
#include "ShaderBase.h"

class LitShader : public Component<LitShader>, public ShaderBase {

public:
    LitShader() {}

    LitShader(const Settings& currentSettings, const std::string& vertexPath, const std::string& fragmentPath, const SHADER_TYPE& type, const std::string& geometryPath = "");

    void recompileShader(const Settings& currentSettings);

    //Must use shader before calling!!!!
    void setPointLight(const Settings& currentSettings, const PointLight& light, unsigned int index);

    //Must use shader before calling!!!!
    void setDirectionalLight(const DirectionalLight& directionalLight);

    //Must use shader before calling!!!!
    void setMaterial(const Material& material);

    //Must use shader before calling!!!!
    void setSimpleMaterial(const SimpleMaterial& material);

    //Must use shader before calling!!!!
    void setShininess(const float& shininess);

private:
    void updateTagValues(const Settings& currentSettings, std::string& fragmentCode);

    void updateTagValue(const std::string& tag, const std::string& toAdd, std::string& stringToModify) {
        std::string strtString = stringToModify.substr(stringToModify.find(tag));
        std::string endString  = strtString.substr(0, strtString.find(Shaders::TAG_END));
        stringToModify.replace(stringToModify.find(endString), endString.length(), tag + toAdd);
    }
};

#endif
