#include "Texture.h"

Texture::Texture(std::string loc, GLuint txture, GLuint w, GLuint h, bool istransparent)
{
	location = loc;
	texture = txture;
	imageHeight = h;
	imageWidth = w;
	isTransparent = istransparent;
}

Texture::Texture()
{
}

Texture & TextureHandler::createTexture(std::string filePath, GLint filtering, bool repeatTexture)
{

	SDL_Surface *surface;
	GLuint		texture;



	Texture * newTexture;

	if ((surface = IMG_Load(filePath.c_str())))
	{
		GLenum		textureFormat;

		GLint		numberOfColors;

		bool		transparent = false;

		#ifdef DEBUG
		if(surface->w >= 4096)
		{
			printf("Image is %d pixels in width, think about scaling down to save memory\n", surface->w);
		}
		if (surface->h >= 4096)
		{
			printf("Image is %d pixels in height, think about scaling down to save memory\n", surface->h);
		}
		
		if(!hh::isPowerOfTwo(surface->h))
		{
			printf("The image height (%d) is either bigger than 8192 or not power of two. Please think about changing dimensions of the image\n", surface->h);
		}
		if (!hh::isPowerOfTwo(surface->w))
		{
			printf("The image width (%d) is either bigger than 8192 or not power of two. Please think about changing dimensions of the image\n", surface->w);
		}
		#endif

		//get number of bytes per pixel (for colors)
		numberOfColors = surface->format->BytesPerPixel;

		//contains an alpha channel
		if (numberOfColors == 4)
		{
			transparent = true;		//Image needs to be rendered as transparent
			if (surface->format->Rmask == 0x000000ff)
			{
				textureFormat = GL_RGBA;
			}
			else
			{
				textureFormat = GL_BGRA;
			}
		}
		else if (numberOfColors == 3) //no alpha channel
		{

			if (surface->format->Rmask == 0x000000ff)
			{
				textureFormat = GL_RGB;
			}
			else
			{
				textureFormat = GL_BGR;
			}

		}
		else
		{
			DBG_LOG("Image is not truecolor\n");
		}

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		if (repeatTexture)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
		
		glTexImage2D(GL_TEXTURE_2D, 0,textureFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

		glGenerateMipmap(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		newTexture = new Texture(filePath, texture, surface->w, surface->h, transparent);

		SDL_FreeSurface(surface);
	}
	else
	{
		DBG_LOG("Image could not load properly, using null texture\n");
		DBG_LOG("The location of the non functioning texture is ");
		DBG_LOG(filePath.c_str());
		DBG_LOG("\n");

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLubyte pixels[] = 
		{
			0,0,0,
			255,255,255,
			0,0,0,
			
			255,255,255,
			0,0,0,
			255,255,255,

			0,0,0,
			255,255,255,
			0,0,0
		};

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3, 3, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		newTexture = new Texture(filePath, texture,3, 3, false);
	}
	return *newTexture;
}

Texture* TextureHandler::binarySearchTextures(const std::string& key)
{
	unsigned int mid = 0;
	unsigned int left = 0;
	unsigned int right = textureLibrary.size();

	while (left < right)
	{
		mid = left + (right - left) / 2;

		if (key > textureLibrary.at(mid)->getLocation())
		{
			left = mid + 1;
		}
		else if (key < textureLibrary.at(mid)->getLocation())
		{
			right = mid;
		}
		else
		{
			return textureLibrary.at(mid);
		}
	}

	return nullptr;
}
void TextureHandler::addThenSort(Texture& texture)
{
	//Add sound to library
	textureLibrary.push_back(&texture);

	//sort the library so I can use a binary search.
	std::sort
	(
		textureLibrary.begin(),
		textureLibrary.end(),
		[](Texture* a, Texture* b)	//oooooo a fancy Lambda expression :)
		{
			return a->getLocation() < b->getLocation();
		}
	);

}
Texture & TextureHandler::addNew(std::string filePath, GLint filtering, bool repeatTexture)
{
	Texture& newTexture = createTexture(filePath,filtering,repeatTexture);
	addThenSort(newTexture);
	return newTexture;
}
Texture& TextureHandler::getTexture(std::string filePath, GLint filtering, bool repeatTexture)
{

	Texture * texture = binarySearchTextures(filePath);
	
	if(!texture)
	{
		texture = &addNew(filePath, filtering, repeatTexture);
	}

	return *texture;

}

TextureHandler::~TextureHandler()
{
	DBG_LOG("freeing memory allocated for image library.\n");

	hh::clearVectorOfPointers(textureLibrary);
}
