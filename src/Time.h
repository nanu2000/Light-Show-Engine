#ifndef TIME_H
#define TIME_H
class Application;

#include <SDL.h>

class Time {

public:
    double getMSPF() const {
        return currentMSPF;
    }
    double getFPS() const {
        return 1.0 / (currentMSPF / 1000.0);
    }
    uint32_t sinceStartMS() const {
        return SDL_GetTicks();
    }
    double sinceStartSD() const {
        return SDL_GetTicks() / 1000.0;
    }
    float sinceStartSF() const {
        return SDL_GetTicks() / 1000.0f;
    }

private:
    double currentMSPF = 0;

    struct TimeData {
        Uint64 then;
        Uint64 now;
        Uint64 frequency;
    };

    TimeData timeData;

    void initialize() {
        timeData.frequency = SDL_GetPerformanceFrequency();
    }

    void updateMSPF();

    friend Application;
};

#endif // !TIME_H
