#include "Window.h"

Window::~Window() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(gameWindow);
}
void Window::initialize() {

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gameWindow = SDL_CreateWindow //create game window
        (
            windowTitle.c_str(), //Sets title of the applications window
            SDL_WINDOWPOS_CENTERED, //sets x position of window
            SDL_WINDOWPOS_CENTERED, //sets y position of window
            width, //sets width of window (pixel size)
            height, //sets height of window (pixel size)
            SDL_WINDOW_OPENGL | //enables window to be used with opengl
                SDL_WINDOW_RESIZABLE);

    glViewport(0, 0, width, height); //openGL camera viewport

    glContext = SDL_GL_CreateContext(gameWindow);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 1);
}

void Window::handleEvents(SDL_Event& eventSystem, SDL_WindowEventID windowEvent, Messenger<BackEndMessages>& msgr) {

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

void Window::setWindowTitle(const std::string& title) {
    windowTitle = title;
    SDL_SetWindowTitle(gameWindow, windowTitle.c_str());
}

void Window::resizeWindow(int w, int h, Messenger<BackEndMessages>& msgr) {
    width  = w;
    height = h;
    SDL_SetWindowSize(gameWindow, width, height);
    glViewport(0, 0, width, height);
    msgr.AddMessage(BackEndMessages::REFRESH_CAMERA);
}

glm::ivec2 Window::getDeviceMaxScreenSize() {
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
