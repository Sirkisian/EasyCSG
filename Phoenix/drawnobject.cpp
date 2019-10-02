#include "drawnobject.hpp"

DrawnObject::DrawnObject():
vao(0),
vbo(0)
{
	if(GLEW_VERSION_3_0)
	{
		glGenVertexArrays(1, &this->vao);
	}

	glGenBuffers(1, &this->vbo);
}

DrawnObject::~DrawnObject()
{
	if(this->vao > 0)
	{
		glDeleteVertexArrays(1, &this->vao);
	}

	if(this->vbo > 0)
	{
		glDeleteBuffers(1, &this->vbo);
	}
}

GLboolean DrawnObject::bindVao()
{
	if(this->vao > 0)
	{
		glBindVertexArray(this->vao);

		return GL_TRUE;
	}

	return GL_FALSE;
}

//unbind is necessary for OpenCSG
GLvoid DrawnObject::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(GLEW_VERSION_3_0)
	{
		glBindVertexArray(0);
	}
	else
	{
		glDisableVertexAttribArray(0);
	}
}