#ifndef TIME_H
#define TIME_H
#include "Debug.h"
#include "GameInfo.h"
#include <SDL.h>

namespace Engine {
    class Application;
}
class Time {

public:
    double getMSPF() const {
        return currentMSPF;
    }
    int getFPS() const {
        return fps;
    }
    uint64_t sinceStartMS32() const {
        return SDL_GetTicks();
    }
    uint64_t sinceStartMS() const {
        return timeSinceStart;
    }
    double sinceStartSD() const {
        return timeSinceStart / 1000.0;
    }
    float sinceStartSF() const {
        return sinceStartMS32() / 1000.0f;
    }

private:
    uint64_t timeSinceStart        = 0;
    int fps                        = 0;
    int currentFramesInSecond      = 0;
    float timeElapsedForFPSCounter = 0;

    double currentMSPF = 0;

    struct TimeData {
        Uint64 then = 0;
        Uint64 now  = 0;
    };

    TimeData timeData;

    void initialize() {

        timeSinceStart = SDL_GetTicks();
        updateMSPF();
    }

    void updateMSPF();
    void updateFPS();

    friend class Engine::Application;
};

#endif // !TIME_H
