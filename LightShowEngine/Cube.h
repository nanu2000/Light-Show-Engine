#ifndef CUBE_SHAPE
#define CUBE_SHAPE

#include "ShaderBase.h"
#include "glm/gtc/constants.hpp"
#include "glm/vec3.hpp"
#include <vector>
#include "glm/gtc/type_ptr.hpp"
#include "Shaders.h"


class CubeShape
{
public:

	void create()
	{

		vertices = {
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};

		GLuint posAttrib = Shaders::getAttribLocation(Shaders::AttribName::Position);

		glGenVertexArrays(1, &VAOID);
		glGenBuffers(1, &vertID);

		glBindVertexArray(VAOID);
		glBindBuffer(GL_ARRAY_BUFFER, vertID);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);



	}

	void render(ShaderBase& shader, Camera& camera)
	{
		GLuint transformLoc = Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ModelMatrix);
		GLuint posAttrib = Shaders::getAttribLocation(Shaders::AttribName::Position);

		glm::mat4 view = glm::mat4(glm::mat3(*camera.getViewMatrix()));


		glBindVertexArray(VAOID);

		glUniformMatrix4fv
		(
			Shaders::getUniformLocation(shader.getProgramID(), Shaders::UniformName::ViewMatrix),
			1,
			GL_FALSE,
			glm::value_ptr(view)
		);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

		glBindBuffer(GL_ARRAY_BUFFER, vertID);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glBindVertexArray(0);
	}

private:
	
	std::vector<GLfloat> vertices;
	GLuint VAOID;
	GLuint vertID;

};

#endif