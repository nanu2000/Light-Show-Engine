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
    double accumulator = GameInfo::fixedDeltaTime; // GameInfo::fixedDeltaTime;	//fixed timestep accumulator
    double frameTime   = 0;

    initialize(); //initialize the application

    /**********************************
	* This loop is the main game loop *
	***********************************/

    while (isRunning) {

        currentTime.updateMSPF();
        currentTime.updateFPS();

        now = SDL_GetTicks();

        frameTime = (now - last) / 1000.0f;

        last = now;

        PrivateGameInfo::deltaTime = static_cast<float>(currentTime.getMSPF()) / 1000.0f;

        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        accumulator += frameTime;

        //Set deltatime

        /*******************************************************
		* Inside this loop is where fixed updating is executed *
		********************************************************/
        while (accumulator >= GameInfo::fixedDeltaTime) {
            fixedUpdate();

            currentTime.timeSinceStart += static_cast<uint64_t>(GameInfo::fixedDeltaTime) * 1000;

            accumulator -= GameInfo::fixedDeltaTime;
        }

        //must be called before render.
        update();
        render();
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

    glewExperimental = GL_TRUE;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

    GLenum err = glewInit();

    if (err != GLEW_OK) {
        DBG_LOG("engine init error (glew): %s (Application.cpp)\n", glewGetErrorString(err));
    }

    glLineWidth(1.5f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);

    //For back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR) {
        DBG_LOG("GL Error: %i (Application.cpp)\n", static_cast<int>(glError));
    }
}

void Application::initialize() {

    initializeSDL(); //initialize SDL2

    initializeAudio(); //initialize SDL_mixer

    initializeGL(); //initialize opengl

    currentTime.initialize();

    std::srand(SDL_GetTicks()); //Set random seed

    InputLocator ::provide(inputService);
    MusicLocator ::provide(musicService);
    SoundLocator ::provide(soundService);
    TextureLocator ::provide(textureService);
    ShaderLocator ::provide(shaderService);

    thisGame.initialize(&currentTime, &backEndMessagingSystem);
}

void Application::fixedUpdate() {

    thisGame.fixedUpdate();
}

void Application::render() {

    thisGame.render();

    SDL_GL_SwapWindow(gameWindow.getWindow());
}

void Application::update() {

    SDL_PumpEvents();

    while (SDL_PollEvent(&sdlEventSystem)) {
        inputService.handleEvents(sdlEventSystem, static_cast<SDL_EventType>(sdlEventSystem.type));

        switch (static_cast<SDL_EventType>(sdlEventSystem.type)) {
        case SDL_EventType::SDL_WINDOWEVENT:
            gameWindow.handleEvents(sdlEventSystem, static_cast<SDL_WindowEventID>(sdlEventSystem.window.event), backEndMessagingSystem);
            break;
        }
    }

    thisGame.update();
}

void Application::uninitialize() {

    thisGame.uninitialize();

    SDL_Quit(); //quit application
}
