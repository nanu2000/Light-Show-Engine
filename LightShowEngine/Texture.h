#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <GL\glew.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <algorithm>
#include "HelpingHand.h"
#include "Debug.h"

class Texture
{

public:
	Texture();
	Texture	( std::string loc, GLuint txture, GLuint w, GLuint h, bool istransparent );

	inline	bool		checkTransparency	()	const {		return isTransparent;	}
	inline	GLuint		getWidth			()	const {		return imageWidth;		}
	inline	GLuint		getHeight			()	const {		return imageHeight;		}
	inline	GLuint		getTextureData		()	const {		return texture;			}
	inline	std::string	getLocation			()	const {		return location;		}

private:
	GLuint		imageWidth		= 0;
	GLuint		imageHeight		= 0;
	GLuint		texture			= 0;
	bool		isTransparent	= false;
	std::string location;

};



class TextureHandler
{

public:

	//Will return Texture if it already exists, but will create one if it doesnt exist.
	Texture& getTexture(std::string filePath, GLint filtering = GL_LINEAR, bool repeatTexture = false);
	
	~TextureHandler();

private:

	Texture &	createTexture(std::string filePath, GLint filtering, bool repeatTexture);

	Texture *	binarySearchTextures		(const std::string& key);
	Texture &	addNew						(std::string filePath, GLint filtering, bool repeatTexture);
	void		addThenSort					(Texture& texture);
	std::vector<Texture*> textureLibrary;
};





#endif
