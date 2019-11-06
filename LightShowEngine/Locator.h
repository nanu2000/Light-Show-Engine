#ifndef LOCATOR_H
#define LOCATOR_H
#include "Input.h"
#include "Sound.h"
#include "Texture.h"
/*************************************************************************************************
It is assumed that U and T are related by inheritance (U inherits T, or they both are the same.) *
**************************************************************************************************/
template <class T, class U>
class Locator {

public:
    Locator() {
        service = &nullService;
    }

    static T& getService() {
        if (service) {
            return *service;
        } else {
            DBG_LOG("Returning null service object(not pointer) because the service is null.\n");
            return nullService;
        }
    }

    static void provide(T& newService) {
        service = &newService;
    }

private:
    static T* service;

    static U nullService;
};

template <class T, class U>
T* Locator<T, U>::service;

template <class T, class U>
U Locator<T, U>::nullService;

class TextureLocator : public Locator<TextureHandler, TextureHandler> {};
class InputLocator : public Locator<Input, NullInput> {};
class SoundLocator : public Locator<SoundHandler, SoundHandler> {};
/*Remember, that if I use the music service and play a song then change the service without stopping the song, the locator will not be able to stop that song.*/
class MusicLocator : public Locator<MusicHandler, MusicHandler> {};

#endif
