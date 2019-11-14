#ifndef _SOUND_H
#define _SOUND_H

#include "DebugDrawer.h"
#include "HelpingHand.h" //included for clearing a vector of pointers.
#include <SDL_mixer.h> //Included for audio
#include <algorithm> //included for sorting
#include <vector> //included for the audio libraries

/***************************************************************************************************************
The Audio class. The Music and Sound class inherit from this. It provides a template to use for audio handling.*
****************************************************************************************************************/
class Audio {

public:
    virtual void initialize(const std::string& location) = 0;

    inline virtual void play()                     = 0;
    inline virtual std::string getLocation() const = 0;
    inline virtual bool isNullSound() const        = 0;

protected:
    std::string location;
};

/***********************************************************************************************************************************************
The Sound class. It derives from the audio class, and is used as a wrapper for SDL_mixer. It handles the basic functionality of a sound effect.*
************************************************************************************************************************************************/
class Sound : public Audio {

public:
    ~Sound();
    Sound();
    inline void play() override;
    inline std::string getLocation() const override;
    inline bool isNullSound() const override;

    void initialize(const std::string& location) override;

private:
    Mix_Chunk* audioClip = nullptr;
};

/**************************************************************************************************************************************
The Music class. It derives from the audio class, and is used as a wrapper for SDL_mixer. It handles the basic functionality of  music*
***************************************************************************************************************************************/
class Music : public Audio {

public:
    ~Music();
    Music();

    inline void play() override;
    inline std::string getLocation() const override;
    inline bool isNullSound() const override;

    inline void fadeIn();

    void togglePlay();

    void togglePlay(bool pause);

    void initialize(const std::string& location) override;

private:
    const unsigned int FADE_SPEED = 2000;

    Mix_Music* soundClip = nullptr;
};

/**********************
AUDIO HANDLING CLASSES*
***********************/

/***********************************************************************
The SoundHandler class. It handles a library of sounds to play and use.*
************************************************************************/
class SoundHandler {

public:
    void addSound(const std::string& location);
    void playSound(const std::string& location);
    ~SoundHandler();

private:
    Sound* binarySearchSounds(const std::string& key);
    void addSoundWithoutChecking(const std::string& location);
    void addThenSort(Sound& sound);

    std::vector<Sound*> soundLibrary;
};

/*******************************************************************************
The MusicHandler class. It handles a library of Music to play, modify, and use.*
********************************************************************************/
class MusicHandler {

public:
    void stop(bool fadeOut);
    void addMusic(const std::string& location);
    void playMusic(const std::string& location, bool fadeIn = false);
    void toggleMusic();
    void toggleMusic(bool paused);
    bool isPlayingMusic();
    ~MusicHandler();

private:
    Music* binarySearchMusic(const std::string& key);
    void addMusicWithoutChecking(const std::string& location);
    void addThenSort(Music& sound);

    std::vector<Music*> musicLibrary;

    const unsigned int FADE_SPEED = 2000;
    Music* currentlyPlaying       = nullptr;
};

#endif _SOUND_H
