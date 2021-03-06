#ifndef TEXT_MAP_H
#define TEXT_MAP_H
#include "Glyph.h"
#include "Serialization.h"
#include "Texture.h"
#include <vector>

class TextMap {

public:
    void createMap(const std::string& textDatMapLocation);

    const Glyph* const getGlyph(const char& key);

    uint32_t getMaxPixelHeightOfAllGlyphs() {
        return maxPixelHeight;
    }

private:
    bool isBigEndianness = false;

    uint32_t maxPixelHeight = 0;

    std::vector<Glyph> map;
};

#endif // !TEXT_MAP_H
