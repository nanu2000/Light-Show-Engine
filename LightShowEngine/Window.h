#ifndef WINDOW_H
#define WINDOW_H
#include <SDL/SDL.h>
#include "GameInfo.h"
#include <string>
#include <glm/vec2.hpp>
#include "Debug.h"
#include <GL/glew.h>
#include "Messenger.h"

class Window
{
	public:

	~Window();

	void initialize();

	void handleEvents	(SDL_Event & eventSystem, SDL_WindowEventID windowEvent, Messenger<BackEndMessages> & msgr);

	void setWindowTitle	(const std::string & title);

	unsigned int		getWidth		() const { return width;		}
	unsigned int		getHeight		() const { return height;		}
	std::string			getCurrentTitle	() const { return windowTitle;	}
	SDL_Window * const  getWindow		() const { return gameWindow;	}
		

	
private:

	void resizeWindow(int w, int h, Messenger<BackEndMessages> & msgr);

	glm::ivec2 getDeviceMaxScreenSize();

	std::string windowTitle = GameInfo::WINDOW_TITLE;
		
	SDL_Window *	gameWindow	= nullptr;
	SDL_GLContext	glContext	= nullptr;	

	unsigned int width	= GameInfo::START_WINDOW_WIDTH;
	unsigned int height = GameInfo::START_WINDOW_HEIGHT;
			
};


#endif // !WINDOW_H
