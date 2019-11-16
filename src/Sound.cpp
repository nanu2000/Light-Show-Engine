#include "Sound.h"

/***********************
*The sound constructor *
************************/
Sound::Sound() {}

/*************************************************************************
*The sound destructor will free the memory associated with the audio clip*
**************************************************************************/
Sound::~Sound() {
    Mix_FreeChunk(audioClip);
    audioClip = nullptr;
}

/******************************************************************************************
*The getLocation function will return the current file path associated with the audio file*
*******************************************************************************************/
std::string Sound::getLocation() const {
    return location;
}

/********************************************************************************************
*The play function will play the audio clip and return true if it exists. false if it doesnt*
*********************************************************************************************/
void Sound::play() {
    if (audioClip) {
        Mix_PlayChannel(-1, audioClip, 0);
    } else {
        DBG_LOG("Sound is null, cannot play.\n");
    }
}

/******************************************************
*Will return true if the audio clip is a null pointer.*
*******************************************************/
bool Sound::isNullSound() const {
    return audioClip == nullptr;
}

/**********************************************************
*This function will initialize the audio file in question.*
***********************************************************/
void Sound::initialize(const std::string& loc) {
    audioClip = Mix_LoadWAV(loc.c_str());
    location  = loc;
}

//******************************************End of Sound Class************************************************

//****************************************Begining of Music Class*********************************************

/********************************************************************************************
*The play function will play the music clip and return true if it exists. false if it doesnt*
*********************************************************************************************/
void Music::play() {
    if (soundClip) {
        Mix_PlayMusic(soundClip, -1);
    } else {
        DBG_LOG("Music is null, cannot play.\n");
    }
}

/******************************************************************************************
*The getLocation function will return the current file path associated with the music file*
*******************************************************************************************/
inline std::string Music::getLocation() const {
    return location;
}

/******************************************************
*Will return true if the music clip is a null pointer.*
*******************************************************/
inline bool Music::isNullSound() const {
    return soundClip == nullptr;
}

inline void Music::fadeIn() {
    if (soundClip) {
        Mix_FadeInMusic(soundClip, -1, FADE_SPEED);
    } else {
        DBG_LOG("Music is null, cannot fade in.\n");
    }
}

/**********************************************************
*This function will initialize the music file in question.*
***********************************************************/
void Music::initialize(const std::string& loc) {
    soundClip = Mix_LoadMUS(loc.c_str());
    location  = loc;
}

/**********************************************************
*This function will toggle if the music is playing or not.*
***********************************************************/
void Music::togglePlay() {
    if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }
}

/**********************************************************
*This function will toggle if the music is playing or not.*
***********************************************************/
void Music::togglePlay(bool pause) {
    if (pause) {
        Mix_PauseMusic();
    } else {
        Mix_ResumeMusic();
    }
}

/********************************************
*The default constructor of the Music class.*
*********************************************/
Music::Music() {}

/********************************************************************************
*The default destructor of the Music class. it will free the audio file's memory*
*********************************************************************************/
Music::~Music() {
    Mix_FreeMusic(soundClip);
    soundClip = nullptr;
}

//******************************************End of Music Class************************************************

//*************************************Begining of SoundHandler Class*****************************************

/************************************************************************************************************************
*The binarySearchSounds function will return the sound pointer associated with the key if it exists in the sorted vector*
*************************************************************************************************************************/
Sound* SoundHandler::binarySearchSounds(const std::string& key) {
    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = soundLibrary.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (key > soundLibrary.at(mid)->getLocation()) {
            left = mid + 1;
        } else if (key < soundLibrary.at(mid)->getLocation()) {
            right = mid;
        } else {
            return soundLibrary.at(mid);
        }
    }

    return nullptr;
}

/*************************************************************************************************************************
*The addThenSort function will add the sound passed by reference to the vector of sound pointers, then re-sort the vector*
**************************************************************************************************************************/
void SoundHandler::addThenSort(Sound& sound) {
    //Add sound to library
    soundLibrary.push_back(&sound);

    //sort the library so I can use a binary search.
    std::sort(
        soundLibrary.begin(),
        soundLibrary.end(),
        [](Sound* a, Sound* b) //oooooo a fancy Lambda expression :)
        {
            return a->getLocation() < b->getLocation();
        });
}

/**************************************************************************************************************************
*The addSoundWithoutChecking function will add a new sound to the library without checking if it is already in the library*
***************************************************************************************************************************/
void SoundHandler::addSoundWithoutChecking(const std::string& location) {

    Sound* newSound = new Sound();
    newSound->initialize(location);

    if (!newSound->isNullSound()) {
        //add the pointer to the sound library because the location worked.
        addThenSort(*newSound);

#ifdef DEBUG
        printf("Sound was Succesfully added to library.\n");
#endif
    } else {

        DBG_LOG("Audio clip is null, could not Add.\nAudio file location is \"");
        DBG_LOG(location.c_str());
        DBG_LOG("\"\nFreeing Allocated Memory.\n");

        //free memory because the location was incorrect.
        delete newSound;
        return;
    }
}

/**************************************************************************************************************************
*The addSound function will add a new sound to the library and check to make sure it doesn't already exist in the library *
***************************************************************************************************************************/
void SoundHandler::addSound(const std::string& location) {
    if (Sound* soundToFind = binarySearchSounds(location)) {
        DBG_LOG("Sound is already in library.\n");
        return;
    } else {
        addSoundWithoutChecking(location);
    }
}

/*************************************************************************************************************
*The playSound function try to play a sound in the library, and if it doesn't exist, it will try to create it*
**************************************************************************************************************/
void SoundHandler::playSound(const std::string& location) {

    if (Sound* soundToFind = binarySearchSounds(location)) {
        soundToFind->play();
    } else {
        addSoundWithoutChecking(location);
        soundToFind = binarySearchSounds(location);
        soundToFind->play();
    }
}

/************************************************************************************
*The destructor of the sound handler will delete the vector of pointers and clear it*
*************************************************************************************/
SoundHandler::~SoundHandler() {
    hh::clearVectorOfPointers(soundLibrary);
    DBG_LOG("Freeing memory for the sound library\n");
}

//*************************************End of Sound Handler Class*********************************************

//*************************************Begining of MusicHandler Class*****************************************

/************************************************************************************************************************
*The binarySearchMusic function will return the Music pointer associated with the key if it exists in the sorted vector *
*************************************************************************************************************************/
Music* MusicHandler::binarySearchMusic(const std::string& key) {
    unsigned int mid   = 0;
    unsigned int left  = 0;
    unsigned int right = musicLibrary.size();

    while (left < right) {
        mid = left + (right - left) / 2;

        if (key > musicLibrary.at(mid)->getLocation()) {
            left = mid + 1;
        } else if (key < musicLibrary.at(mid)->getLocation()) {
            right = mid;
        } else {
            return musicLibrary.at(mid);
        }
    }

    return nullptr;
}

/*************************************************************************************************************************
*The addThenSort function will add the music passed by reference to the vector of sound pointers, then re-sort the vector*
**************************************************************************************************************************/
void MusicHandler::addThenSort(Music& music) {
    //Add sound to library
    musicLibrary.push_back(&music);

    //sort the library so I can use a binary search.
    std::sort(
        musicLibrary.begin(),
        musicLibrary.end(),
        [](Music* a, Music* b) //oooooo a fancy Lambda expression :)
        {
            return a->getLocation() < b->getLocation();
        });
}

/**************************************************************************************************************************
*The addMusicWithoutChecking function will add a new Music to the library without checking if it is already in the library*
***************************************************************************************************************************/
void MusicHandler::addMusicWithoutChecking(const std::string& location) {

    Music* newMusic = new Music();
    newMusic->initialize(location);

    if (!newMusic->isNullSound()) {
        //add the pointer to the sound library because the location worked.
        addThenSort(*newMusic);

#ifdef DEBUG
        printf("Music was Succesfully added to library.\n");
#endif
    } else {
#ifdef DEBUG
        printf("Music clip is null, could not Add.\nMusic file location is \"");
        printf("%s", location.c_str());
        printf("\"\nFreeing Allocated Memory.\n");
#endif

        //free memory because the location was incorrect.
        delete newMusic;
        return;
    }
}

void MusicHandler::stop(bool fadeOut) {
    if (fadeOut) {
        Mix_FadeOutMusic(FADE_SPEED);
    } else {
        Mix_HaltMusic();
    }
}

/**************************************************************************************************************************
*The addMusic function will add a new Music to the library and check to make sure it doesn't already exist in the library *
***************************************************************************************************************************/
void MusicHandler::addMusic(const std::string& location) {
    if (Music* musicToFind = binarySearchMusic(location)) {
        DBG_LOG("Music is already in library.\n");
        return;
    } else {
        addMusicWithoutChecking(location);
    }
}

/*************************************************************************************************************
*The playMusic function try to play a music in the library, and if it doesn't exist, it will try to create it*
**************************************************************************************************************/
void MusicHandler::playMusic(const std::string& location, bool fadeIn) {
    currentlyPlaying = binarySearchMusic(location);

    if (currentlyPlaying == nullptr) {
#ifdef DEBUG
        printf("music was not found in library. Creating new clip.\n");
#endif
        addMusicWithoutChecking(location);
        currentlyPlaying = binarySearchMusic(location);
    }

    if (fadeIn) {
        currentlyPlaying->fadeIn();
    } else {
        currentlyPlaying->play();
    }
}

/************************************************************************************
*The toggleMusic function will try to toggle the activity of the music if it exists.*
*************************************************************************************/
void MusicHandler::toggleMusic() {
    if (currentlyPlaying) {
        currentlyPlaying->togglePlay();
    } else {
        DBG_LOG("Trying to Toggle activity of music that is either null, or does not exist.\n");
    }
}

/************************************************************************************
*The toggleMusic function will try to toggle the activity of the music if it exists.*
*************************************************************************************/
void MusicHandler::toggleMusic(bool paused) {
    if (currentlyPlaying) {
        currentlyPlaying->togglePlay(paused);
    } else {
#ifdef DEBUG
        printf("Trying to Toggle activity of music that is either null, or does not exist.\n");
#endif
    }
}

bool MusicHandler::isPlayingMusic() {
    return Mix_PlayingMusic() ? true : false;
}

/************************************************************************************
*The destructor of the music handler will delete the vector of pointers and clear it*
*************************************************************************************/
MusicHandler::~MusicHandler() {
    hh::clearVectorOfPointers(musicLibrary);

    DBG_LOG("Freeing memory for the music library\n");
}

//*************************************End of Music Handler Class*********************************************
