#ifndef WINDOW_H
#define WINDOW_H
#include "Debug.h"
#include "Messenger.h"
#include <GL/glew.h>
#include <SDL.h>
#include <glm/vec2.hpp>
#include <string>

namespace Engine {

    //! Handles all of the window logic for the application.
    class Window {
    public:
        Window(unsigned int w, unsigned int h, const std::string& title);
        Window() {}
        ~Window();

        //!Creates window
        void initialize();

        //!Handles sdl events such as resizing
        void handleEvent(SDL_Event& eventSystem, SDL_WindowEventID windowEvent, Messenger<BackEndMessages>& msgr);

        void setWindowTitle(const std::string& title);

        unsigned int getWidth() const { return width; }
        unsigned int getHeight() const { return height; }
        std::string getCurrentTitle() const { return windowTitle; }
        SDL_Window* const getWindow() const { return gameWindow; }

    private:
        void resizeWindow(int w, int h, Messenger<BackEndMessages>& msgr);

        //!Returns max screen size
        glm::ivec2 getDeviceMaxScreenSize();
        std::string windowTitle = "";

        SDL_Window* gameWindow  = nullptr;
        SDL_GLContext glContext = nullptr;

        unsigned int width  = 640;
        unsigned int height = 400;
    };
}
#endif // !WINDOW_H
