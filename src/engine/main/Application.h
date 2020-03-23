#ifndef GAME_APP_H
#define GAME_APP_H

#include "Game.h"
#include "Window.h"
#include <GL/glew.h>

//!Contains the engine side of things.
namespace Engine {

    //!The game loop will only run if this is true.
    extern bool isRunning;

    //!The window used for the game.
    extern Engine::Window gameWindow;

    inline unsigned int const getGameWindowWidth() {
        return gameWindow.getWidth();
    };

    inline unsigned int const getGameWindowHeight() {
        return gameWindow.getHeight();
    };

    inline static void setMousePosition(int xPos, int yPos) {
        SDL_WarpMouseInWindow(gameWindow.getWindow(), xPos, yPos);
    }

    //! The Application class manages important tasks like creating a Game, managing the Window and Locators (single instance services) such as Input, Textures, Audio, and Shaders.
    class Application {

    public:
        void run(); //The function that runs everything

    private:
        //!For Fixed updating - generally physics. Capped to run at GameInfo::fixedDeltaTime
        void fixedUpdate();

        //!For updating- generally user input or ai
        void update();

        //!For Rendering
        void render();

        //!For Initializing everything
        void initialize();

        //!For Uninitializing everything
        void uninitialize();

        //!For initializing openGL
        void initializeGL();

        //!For initializing SDL2
        void initializeSDL();

        //!For initializing audio (SDL_Mixer)
        void initializeAudio();

        SDL_Event sdlEventSystem;

        //!Audio Rate.(frequency) 22050 for LQ sound (faster) 44100 for HQ sound
        unsigned int AUDIO_RATE = 44100;

        //!Audio Channel	2 for sterio, 1 for mono
        unsigned int AUDIO_CHANNEL = 1;

        //!Audio Bufffer (chunk size) Bytes used per output sample
        unsigned int AUDIO_BUFFER = 256;

        //!Audio Format signed 8-bit samples, in system byte order. Same as AUDIO_S16SYS
        unsigned short AUDIO_FORMAT = AUDIO_U8;

        //!The default color of the screen.
        glm::vec4 SCREEN_COLOR = glm::vec4(.8f, .9f, .95f, 1.f);

        //For the InputLocator
        Input inputService;

        //For the SoundLocator
        SoundHandler soundService;

        //For the MusicLocator
        MusicHandler musicService;

        //For the TextureLocator
        TextureHandler textureService;

        //For the ShaderLocator
        ShaderHandler shaderService;

        //!Manages time
        Time currentTime;

        //!Manages BackEndMessages to send to Game
        Messenger<BackEndMessages> backEndMessagingSystem;

        //!We run the game with this!
        Game thisGame;
    };
}
#endif
