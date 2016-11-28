#ifndef GUI_STRING_H
#define GUI_STRING_H
#include "GuiTextCharacter.h"
#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include "TextMap.h"
#include "GuiBase.h"

class GuiString : public GuiBase
{

public:

	void initialize(TextMap & textMap, Texture & texture);

	GuiString(unsigned int capacity = 5, const std::string& string = "");

	void render(ShaderBase& shader);
	void render(ShaderBase& shader, const std::string& string);
	void render(ShaderBase& shader, const std::string& string, unsigned int newCapacity);

	void render
	(
		ShaderBase& shader,
		int			horizontalpad,
		int			verticalpad		= 1,
		int			spacesize		= 8, 
		const glm::ivec2&			= glm::ivec2(0)
	);

	void render
	(
		ShaderBase			& shader,
		const std::string	& string,
		int					horizontalpad,
		int					verticalpad		= 1,
		int					spacesize		= 8, 
		const glm::ivec2&	position		= glm::ivec2(0)
	);
	
	void setTexture				(Texture & texture)				{ currentTexture	= &texture;	}
	void setTextMap				(TextMap & textmap)				{ currentTextMap	= &textmap;	}
	void setPosition			(const glm::vec2& position)		{ textPosition		= position;	}
	void setScale				(const glm::vec2& scl)			{ scale				= scl;		}
	void setString				(const std::string& string)		{ currentString		= string;	}
	void setCapacity			(unsigned int capacity)			{ characters.resize(capacity);	}
	void setHorizontalPadding	(int hPadding = 2)				{ horizontalPadding = hPadding;	}
	void setVerticalPadding		(int vPadding = 1)				{ verticalPadding	= vPadding;	}
	void setSpaceSize			(int spacesize = 8)				{ spaceSize			= spacesize;}

	const std::string * const getString	()
	{ 
		return &currentString;		
	}
	
	const int getHorizontalPadding		() const
	{ 
		return horizontalPadding;	
	}
	
	const int getVerticalPadding		() const
	{
		return verticalPadding;	
	}

	const int getSpaceSize				() const
	{
		return spaceSize;			
	}

	const glm::ivec2 getPosition		() const
	{
		return textPosition;
	}

	const glm::vec2 * const getScale	()
	{
		return &scale;			
	}

	float getWidthOfString	() const { return widthOfString; }


private:
	

	void mainRender(ShaderBase& shader);

	float widthOfString = 0;

	int			horizontalPadding	= 2;
	int			verticalPadding		= 1;
	int			spaceSize			= 8;

	glm::vec2	scale		 = glm::vec2  (1, 1);
	glm::vec2	textPosition = glm::vec2 (0, 0);


	Texture * currentTexture	= nullptr;
	TextMap * currentTextMap	= nullptr;

	std::string currentString;
	std::vector<GuiTextCharacter> characters;


};

#endif // !GUI_STRING_H
