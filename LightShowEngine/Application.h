#ifndef GAME_APP_H
#define GAME_APP_H

/***************************
* Includes and namespaces. *
****************************/
#include "Debug.h"
#include "Game.h"
#include "Window.h"
#include <GL/glew.h>

/****************************************************************************
************************* The application class *****************************
*****************************************************************************/
class Application {

    /******************************************
************* Public Members **************
*******************************************/

public:
    /**************************
	* public Member Functions *
	***************************/
    void run(); //A function that runs everything important

    inline static unsigned int const getGameWindowWidth() {
        return gameWindow.getWidth();
    };
    inline static unsigned int const getGameWindowHeight() {
        return gameWindow.getHeight();
    };
    inline static void setMousePosition(int xPos, int yPos) {
        SDL_WarpMouseInWindow(gameWindow.getWindow(), xPos, yPos);
    }
    /**************************
	* public Member variables *
	***************************/

    static bool isRunning; //The game loop will only run if this is true.

    /******************************************
************* Private Members *************
*******************************************/

private:
    static Window gameWindow;
    /***************************
	* Private Member Functions *
	****************************/

    void fixedUpdate(); //For Important Fixed Updating

    void render(); //For Important Rendering

    void initialize(); //For Important Initializing

    void uninitialize(); //For Important Uninitializing

    void initializeGL(); //for initializing openGL

    void initializeSDL(); //For initializing SDL2

    void initializeAudio(); //For initializing audio (SDL_Mixer)

    /***************************
	* Private Member Variables *
	****************************/

    SDL_Event sdlEventSystem;

    const unsigned int AUDIO_RATE = 22050; //Audio Rate.(frequency) 22050 for LQ sound (faster) 44100 for HQ sound

    const unsigned int AUDIO_CHANNEL = 1; //Audio Channel	2 for sterio, 1 for mono

    const unsigned int AUDIO_BUFFER = 256; //Audio Bufffer (chunk size) Bytes used per output sample

    const unsigned short AUDIO_FORMAT = AUDIO_U8; //Audio Format signed 8-bit samples, in system byte order. Same as AUDIO_S16SYS

    const glm::vec4 SCREEN_COLOR = glm::vec4(.8f, .9f, .95f, 1.f); //The default color of the screen.

    /***********************************************
	*Everything below is used for testing purposes.*
	************************************************/

    Input input; //For the InputLocator

    SoundHandler mainSound; //For the SoundLocator

    MusicHandler mainMusic; //For the MusicLocator

    TextureHandler textureservice; //For the TextureLocator

    Time currentTime;

    Messenger<BackEndMessages> backEndMessagingSystem;

    Game thisGame;
};

#endif
