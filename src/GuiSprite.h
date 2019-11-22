#ifndef GUI_SPRITE_H
#define GUI_SPRITE_H

#include "GuiBase.h"
#include "Quad.h"

class GuiSprite : public GuiBase {

public:
    GuiSprite() {}

    GuiSprite(Texture& txtre) {
        initialize(txtre);
    }

    void initialize(Texture& txtre) {
        texture = &txtre;

        spriteWidth  = (float)texture->getWidth();
        spriteHeight = (float)texture->getHeight();

        scale    = glm::vec2(spriteWidth, spriteHeight);
        position = glm::vec2(0, 0);
    }

    void setSprite(
        float spriteXlocPXL,
        float spriteYlocPXL,
        float spriteWPXL,
        float spriteHPXL) {
        spriteXStart = spriteXlocPXL;
        spriteYStart = spriteYlocPXL;
        spriteWidth  = spriteWPXL;
        spriteHeight = spriteHPXL;
    }

    void setScale(float widthInPixels, float heightInPixels) {
        scale = glm::vec2(widthInPixels, heightInPixels);
    }

    void setScale(const glm::vec2& scaleInPixels) {
        scale = scaleInPixels;
    }

    void setPosition(float x, float y) {
        position = glm::vec2(x, y);
    }

    void setPosition(const glm::vec2& pos) {
        position = pos;
    }

    void render(ShaderBase& shader) {
        if (texture == nullptr) {

            DBG_LOG("GuiSprite needs to be initialized before being used. The texture must not be null.");

            assert(texture);
        }

        GLboolean currentDepth;
        glGetBooleanv(GL_DEPTH_WRITEMASK, &currentDepth);

        glDepthMask(GL_FALSE);
        quad.render3D(shader, *texture, glm::vec4(spriteXStart, spriteYStart, spriteWidth, spriteHeight), position, scale);
        glDepthMask(currentDepth);
    }

protected:
    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 scale    = glm::vec2(0.0f, 0.0f);

    float spriteXStart = 0;
    float spriteYStart = 0;

    float spriteWidth  = 0;
    float spriteHeight = 0;

    Texture* texture = nullptr;
    Quad quad;
};

#endif
