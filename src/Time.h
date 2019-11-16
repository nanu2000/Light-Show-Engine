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
    uint64_t timeSinceStart = 0;

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
