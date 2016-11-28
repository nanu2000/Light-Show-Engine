#include "Serialization.h"

bool SL::isBigEndian()
{
	FourBytes x = 1;
	
	return *reinterpret_cast<Byte*>(&x) == 0;
}

void SL::writeBytes(const char * cStr, const unsigned short & amountOfBytes, std::ofstream & oStream)
{
	oStream.write(cStr, amountOfBytes);
}

void SL::readBytes(const char * cStr, const unsigned short & amountOfBytes, std::ifstream & iStream)
{
	iStream.read(const_cast<char*>(cStr), amountOfBytes);
}