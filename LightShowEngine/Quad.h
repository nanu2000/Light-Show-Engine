#ifndef QUAD_H
#define QUAD_H
#include <GL/glew.h>
#include "Shaders.h"
#include "ShaderBase.h"
#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "GameInfo.h"
#include "Transform.h"
class Quad
{
public:

	void RenderQuad
	(
		const ShaderBase & shader,
		const Texture & texture,
		const glm::vec4& spriteOrientationInPixels,
		const glm::vec2& positionInPixels,
		const glm::vec2& scaleInPixels
	);

private:
	
	GLint positionAttribLocation;
	GLint textureCoordsAttribLocation;

	GLuint quadVAO = 0;
	GLuint quadVBO;
};
#endif