#include "SphereShape.h"

SphereShape SphereShape::createSphere(int radius, int stacks, int slices)
{
	SphereShape newShape;

	      int i, j;
	      for (i = 0; i <= stacks; i++) {
	          double lat0 = glm::pi<float>() * (-0.5 + (double)(i - 1) / stacks);
	          double z0 = sin(lat0);
	          double zr0 = cos(lat0);

	          double lat1 = glm::pi<float>() * (-0.5 + (double)i / stacks);
	          double z1 = sin(lat1);
	          double zr1 = cos(lat1);

	          glBegin(GL_QUAD_STRIP);
	          for (j = 0; j <= slices; j++) {
	              double lng = 2 * glm::pi<float>() * (double)(j - 1) / slices;
	              double x = cos(lng);
	              double y = sin(lng);

				  newShape.vertices.push_back(glm::vec3(x * zr0, y * zr0, z0));
				  newShape.vertices.push_back(glm::vec3(x * zr1, y * zr1, z1));
	
		}
	          glEnd();
	
	}



	glGenVertexArrays(1, &newShape.VAOID);
	glBindVertexArray(newShape.VAOID);
	glGenBuffers(1, &newShape.vertID);
	glGenBuffers(1, &newShape.indID);

	glBindBuffer(GL_ARRAY_BUFFER, newShape.vertID);
	glBufferData(GL_ARRAY_BUFFER, newShape.vertices.size() * sizeof(glm::vec3), &newShape.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, newShape.indID);
	glBufferData(GL_ARRAY_BUFFER, newShape.indices.size() * sizeof(unsigned int), &newShape.indices[0], GL_STATIC_DRAW);



	return newShape;
}

void SphereShape::drawSphere(ShaderBase & shader)
{
	glBindVertexArray(VAOID);
	GLuint transformLoc = glGetUniformLocation(shader.getProgramID(), "model");

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4()));

	GLint posAttrib = glGetAttribLocation(shader.getProgramID(), "position");


	glEnableVertexAttribArray(posAttrib);

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, indID);

	glBindBuffer(GL_ARRAY_BUFFER, vertID);

	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
}
