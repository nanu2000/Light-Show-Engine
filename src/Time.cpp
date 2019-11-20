#include "Time.h"

void Time::updateMSPF() {

    timeData.then = timeData.now;
    timeData.now  = SDL_GetPerformanceCounter();

    //convert nanoseconds to milliseconds;
    currentMSPF = (timeData.now - timeData.then) * 1000 / static_cast<double>(SDL_GetPerformanceFrequency());
}

void Time::updateFPS() {
    timeElapsedForFPSCounter += GameInfo::getDeltaTime();

    currentFramesInSecond++;

    if (timeElapsedForFPSCounter >= 1.0f) {
        fps                      = currentFramesInSecond;
        currentFramesInSecond    = 0;
        timeElapsedForFPSCounter = 0;
    }
}
