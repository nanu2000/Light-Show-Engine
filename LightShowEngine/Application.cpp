#include "Application.h"
bool Application::isRunning = true; //Set to true so the game runs

Window Application::gameWindow = Window();

void GameInfo::terminateGame() {
    Application::isRunning = false;
}

int GameInfo::getWindowWidth() {
    return Application::getGameWindowWidth();
}

int GameInfo::getWindowHeight() {
    return Application::getGameWindowHeight();
}

void GameInfo::setMousePosition(int xPos, int yPos) {
    Application::setMousePosition(xPos, yPos);
}

void Application::run() {
    unsigned long now  = SDL_GetTicks(); //milliseconds passed
    unsigned long last = now; //last known amount of milliseconds passed since last update
    double accumulator = GameInfo::deltaTime; // GameInfo::deltaTime;	//fixed timestep accumulator
    double frameTime   = 0;

    initialize(); //initialize the application

    /**********************************
	* This loop is the main game loop *
	***********************************/

    currentTime.timeSinceStart = SDL_GetTicks();
    while (isRunning) {

        now = SDL_GetTicks();

        frameTime = (now - last) / 1000.0f;

        last = now;

        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        accumulator += frameTime;

        /*******************************************************
		* Inside this loop is where fixed updating is executed *
		********************************************************/
        while (accumulator >= GameInfo::deltaTime) {
            fixedUpdate();

            currentTime.timeSinceStart += static_cast<uint64_t>(GameInfo::deltaTime * 1000);

            accumulator -= GameInfo::deltaTime;
        }

        render();

        currentTime.updateMSPF();
    }

    uninitialize(); //uninitialize the application
}

void Application::initializeAudio() {
    if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNEL, AUDIO_BUFFER) != 0) {
        DBG_LOG("There was an issue initializing SDL_mixer.\n");
    }
}

void Application::initializeSDL() {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        DBG_LOG("There was an issue initializing SDL.\n");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gameWindow.initialize();

    SDL_SetWindowMinimumSize(gameWindow.getWindow(), GameInfo::MIN_WINDOW_WIDTH, GameInfo::MIN_WINDOW_HEIGHT);

    GameInfo::setMousePosition(GameInfo::getWindowWidth() / 2, GameInfo::getWindowHeight() / 2);
}

void Application::initializeGL() {

    glClearColor //sets the color of the screen.
        (
            SCREEN_COLOR.x, //red
            SCREEN_COLOR.y, //green
            SCREEN_COLOR.z, //blue
            SCREEN_COLOR.w //alpha
        );

    glewExperimental = GL_TRUE;

    glewInit();

    glLineWidth(1.5f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);

    //For back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Application::initialize() {

    initializeSDL(); //initialize SDL2

    initializeAudio(); //initialize SDL_mixer

    initializeGL(); //initialize opengl

    currentTime.initialize();

    std::srand(SDL_GetTicks()); //Set random seed

    InputLocator ::provide(input);
    MusicLocator ::provide(mainMusic);
    SoundLocator ::provide(mainSound);
    TextureLocator ::provide(textureservice);

    thisGame.initialize(&currentTime, &backEndMessagingSystem);
}

void Application::fixedUpdate() {

    SDL_PumpEvents();

    while (SDL_PollEvent(&sdlEventSystem)) {
        input.handleEvents(sdlEventSystem, static_cast<SDL_EventType>(sdlEventSystem.type));

        switch (static_cast<SDL_EventType>(sdlEventSystem.type)) {
        case SDL_EventType::SDL_WINDOWEVENT:
            gameWindow.handleEvents(sdlEventSystem, static_cast<SDL_WindowEventID>(sdlEventSystem.window.event), backEndMessagingSystem);
            break;
        }
    }

    thisGame.fixedUpdate();
}

void Application::render() {

    thisGame.render();

    SDL_GL_SwapWindow(gameWindow.getWindow());
}

void Application::uninitialize() {

    thisGame.uninitialize();

    SDL_Quit(); //quit application
}
