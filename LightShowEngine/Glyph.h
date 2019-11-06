#ifndef GLYPH_H
#define GLYPH_H
#include <stdint.h>
struct Glyph {
    uint32_t x;
    uint32_t xMax;
    uint32_t y;
    uint32_t yMax;
    char character;

    uint32_t getWidth() const {
        return xMax - x + 1;
    }

    uint32_t getHeight() const {
        return yMax - y;
    }
};

#endif // !GLYPH_H
