#include "Window.h"
Engine::Window::Window(unsigned int w, unsigned int h) {
    width  = w;
    height = h;
}
Engine::Window::~Window() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(gameWindow);
}
void Engine::Window::initialize() {

    gameWindow = SDL_CreateWindow //create game window
        (windowTitle.c_str(),
         SDL_WINDOWPOS_CENTERED, //sets x position of window
         SDL_WINDOWPOS_CENTERED, //sets y position of window
         width,
         height,
         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE); //enables window to be used with opengl and to be resizable.

    glViewport(0, 0, width, height); //openGL camera viewport

    glContext = SDL_GL_CreateContext(gameWindow);
}

void Engine::Window::handleEvents(SDL_Event& eventSystem, SDL_WindowEventID windowEvent, Messenger<BackEndMessages>& msgr) {

    switch (windowEvent) {

    case SDL_WINDOWEVENT_MAXIMIZED: {
        glm::ivec2 current = getDeviceMaxScreenSize();
        resizeWindow(current.x, current.y, msgr);
        DBG_LOG("Maximizing Window\n");
    } break;

    case SDL_WINDOWEVENT_RESIZED: {
        resizeWindow(eventSystem.window.data1, eventSystem.window.data2, msgr);
        DBG_LOG("Resizing Window\n");
    } break;
    }
}

void Engine::Window::setWindowTitle(const std::string& title) {
    windowTitle = title;
    SDL_SetWindowTitle(gameWindow, windowTitle.c_str());
}

void Engine::Window::resizeWindow(int w, int h, Messenger<BackEndMessages>& msgr) {
    width  = w;
    height = h;
    SDL_SetWindowSize(gameWindow, width, height);
    glViewport(0, 0, width, height);
    msgr.addMessage(BackEndMessages::REFRESH_CAMERA);
}

glm::ivec2 Engine::Window::getDeviceMaxScreenSize() {
    SDL_DisplayMode current;
    int window = 0;

    window = SDL_GetCurrentDisplayMode(0, &current);

    if (window != 0) {
        DBG_LOG("THERE WAS AN ERROR READING THE CURRENT DEVICES SCREEN WIDTH AND HEIGHT. (Window.h getDeviceScreenSize())\n ");
    } else {
        return glm::ivec2(current.w, current.h);
    }

    return glm::ivec2(-1, -1);
}
