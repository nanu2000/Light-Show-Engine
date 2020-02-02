#include "GuiString.h"

void GuiString::initialize(TextMap& textMap, Texture& texture) {
    currentTextMap = &textMap;
    currentTexture = &texture;
}

GuiString::GuiString(unsigned int capacity, const std::string& string) {
    characters.resize(capacity);

    if (!string.empty()) {
        currentString = string;
    }
}

void GuiString::render(Shader& shader) {
    mainRender(shader);
}

void GuiString::render(Shader& shader, const std::string& string) {
    currentString = string;
    mainRender(shader);
}

void GuiString::render(
    Shader& shader,
    const std::string& string,
    int horizontalpad,
    int verticalpad,
    int spacesize,
    const glm::ivec2& position) {

    horizontalPadding = horizontalpad;
    verticalPadding   = verticalpad;
    spaceSize         = spacesize;
    textPosition      = position;
    currentString     = string;

    mainRender(shader);
}

void GuiString::render(Shader& shader, const std::string& string, unsigned int newCapacity) {
    characters.resize(newCapacity);
    currentString = string;
    mainRender(shader);
}

void GuiString::render(Shader& shader, int horizontalpad, int verticalpad, int spacesize, const glm::ivec2&) {

    horizontalPadding = horizontalpad;
    verticalPadding   = verticalpad;
    spaceSize         = spacesize;

    mainRender(shader);
}

void GuiString::mainRender(Shader& shader) {
    if (!currentTexture) {
        DBG_LOG("Current Font Texture is Null (GuiString::render())\n");
        return;
    }
    if (!currentTextMap) {
        DBG_LOG("Current Text Map is Null (GuiString::render())\n");
        return;
    }

    float currentX    = 0;
    float currentY    = 0;
    int indexModifier = 0;

    widthOfString = 0;

    GLboolean currentDepth;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &currentDepth);
    glDepthMask(GL_FALSE);

    for (unsigned int i = 0; i < characters.size() && i + indexModifier < currentString.size(); i++) {

        const char currentChar = currentString.at(i + indexModifier);

        const Glyph* currentGlyph = currentTextMap->getGlyph(currentChar);

        if (currentChar == '\n') {
            currentX = 0;
            currentY -= static_cast<int>(currentTextMap->getMaxPixelHeightOfAllGlyphs() * scale.y) + verticalPadding;

            indexModifier++;
            i--;
        } else if (currentChar == ' ') {
            currentX += spaceSize;
            indexModifier++;
            i--;
        } else {
            if (currentGlyph) {

                characters[i].xPosition = textPosition.x + currentX * 2 + currentGlyph->getWidth() * scale.x;
                characters[i].yPosition = textPosition.y + currentY * 2 - currentGlyph->getHeight() * scale.y;

                characters[i].render(shader, *currentTexture, *currentGlyph, scale);
                currentX += currentGlyph->getWidth() * scale.x + horizontalPadding;
            }
        }

        if (widthOfString < currentX) {
            widthOfString = currentX;
        }
    }

    glDepthMask(currentDepth);
}
