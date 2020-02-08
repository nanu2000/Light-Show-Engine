#include "Serialization.h"

bool Serialization::isBigEndian() {
    FourBytes x = 1;

    return *reinterpret_cast<Byte*>(&x) == 0;
}

void Serialization::writeBytes(const char* cStr, const unsigned short& amountOfBytes, std::ofstream& oStream) {
    oStream.write(cStr, amountOfBytes);
}

void Serialization::readBytes(const char* cStr, const unsigned short& amountOfBytes, std::ifstream& iStream) {
    iStream.read(const_cast<char*>(cStr), amountOfBytes);
}
