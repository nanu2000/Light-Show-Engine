#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <algorithm>
#include <fstream>

//////////////////////////////////////////////////////////////////////////////////
//I AM EXPECTING A DOUBLE TO BE EQUAL TO 8 BYTES, AND A FLOAT EQUAL TO 4 BYTES	//
//I AM ALSO EXPECTING THE SYSTEM TO READ IN LITTLE ENDIANESS.					//
//////////////////////////////////////////////////////////////////////////////////

namespace SL {

//Typedefs
typedef uint64_t EightBytes;
typedef uint32_t FourBytes;
typedef uint16_t TwoBytes;
typedef uint8_t Byte;

const Byte SIZE_OF_FLOAT = 4;

//Swaps byte order for serialization.
template <typename T>
void swapEndian(T* objp) {
    Byte* memoryLocation = reinterpret_cast<Byte*>(objp);
    std::reverse(memoryLocation, memoryLocation + sizeof(T));
}

template <typename T>
void writeBytes(T& variableToSerialize, const unsigned short& amountOfBytes, std::ofstream& oStream) {
    oStream.write(reinterpret_cast<char*>(&variableToSerialize), amountOfBytes);
}

template <typename T>
void readBytes(T& variableToReadInto, const unsigned short& amountOfBytes, std::ifstream& iStream, const bool& swapEndianness) {
    iStream.read(reinterpret_cast<char*>(&variableToReadInto), amountOfBytes);

    if (swapEndianness) {
        SL::swapEndian(&variableToReadInto);
    }
}

//Returns true if system is big endian. False otherwise.
bool isBigEndian();

void writeBytes(const char* cStr, const unsigned short& amountOfBytes, std::ofstream& oStream);

void readBytes(const char* cStr, const unsigned short& amountOfBytes, std::ifstream& iStream);

}

#endif // !SERIALIZATION_H
