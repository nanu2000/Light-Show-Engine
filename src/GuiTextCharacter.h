#ifndef GUI_TEXT_CHARACTER
#define GUI_TEXT_CHARACTER
#include "Glyph.h"
#include "Quad.h"
class GuiTextCharacter {
public:
    void render(Shader& shader, Texture& texture, const Glyph& glyph, const glm::vec2& scale);

    float xPosition = 0;
    float yPosition = 0;

private:
    Quad quad;
};

#endif // !GUI_TEXT_CHARACTER
