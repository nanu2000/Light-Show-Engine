#include "Time.h"

void Time::updateMSPF() {

    //convert nanoseconds to milliseconds;
    currentMSPF = (timeData.now - timeData.then) / static_cast<double>(timeData.frequency) * 1000;

    timeData.then = timeData.now;
    timeData.now  = SDL_GetPerformanceCounter();
}
