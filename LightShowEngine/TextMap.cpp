#include "TextMap.h"

const Glyph* const TextMap::getGlyph(const char& key) {
    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = map.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (key > map.at(mid).character) {
            left = mid + 1;
        } else if (key < map.at(mid).character) {
            right = mid;
        } else {
            return &map.at(mid);
        }
    }

    return nullptr;
}

void TextMap::createMap(const std::string& textDatLocation) {
    isBigEndianness = SL::isBigEndian();

    std::ifstream inputFileStream(textDatLocation.c_str(), std::ios::in | std::ios::binary);

    uint32_t sizeOfMap = 0;

    inputFileStream.read(reinterpret_cast<char*>(&sizeOfMap), 4);

    map.resize(sizeOfMap);

    for (uint32_t i = 0; i < sizeOfMap; i++) {
        Glyph glyph;

        SL::readBytes(glyph.x, 4, inputFileStream, isBigEndianness);
        SL::readBytes(glyph.xMax, 4, inputFileStream, isBigEndianness);
        SL::readBytes(glyph.y, 4, inputFileStream, isBigEndianness);
        SL::readBytes(glyph.yMax, 4, inputFileStream, isBigEndianness);
        SL::readBytes(glyph.character, 1, inputFileStream, isBigEndianness);

        map[i] = glyph;

        if (glyph.getHeight() > maxPixelHeight) {
            maxPixelHeight = glyph.getHeight();
        }
    }
    std::sort(
        map.begin(),
        map.end(),
        [](Glyph& a, Glyph& b) //oooooo a fancy Lambda expression :)
        {
            return a.character < b.character;
        });
}
