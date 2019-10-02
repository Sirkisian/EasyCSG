#include "axis.hpp"

const GLfloat Axis::alpha = 0.95f;

Axis::Axis(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType)):
SceneObject(vertices, primitiveType),
lineWidth(1.0f)
{
	if(this->bindVao())
	{
		this->bindVbo();
		this->unbind();
	}
}

Axis::~Axis()
{
}

GLvoid Axis::drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode, GLboolean fixedPipeline)
{
	if(!this->bindVao())
	{
		this->bindVbo();
	}

	glLineWidth(this->lineWidth);

	this->setShaderModelVar(shaderProgram);

	GLint location = glGetUniformLocation(shaderProgram, "color");

	size_t array = this->vertices->size() / 3;
	for(GLubyte i = 0; i < 3; i++)
	{
		switch(i)
		{
			case 0:
				{
					glUniform4f(location, 1.0f, 0.0f, 0.0f, Axis::alpha);
				}
				break;

			case 1:
				{
					glUniform4f(location, 0.0f, 1.0f, 0.0f, Axis::alpha);
				}
				break;

			case 2:
				{
					glUniform4f(location, 0.0f, 0.0f, 1.0f, Axis::alpha);
				}
				break;
		}

		if(this->primitiveType != GL_LINES)
		{
			glPolygonMode(GL_FRONT, GL_FILL);
		}

		glDrawArrays(this->primitiveType, (array * i), array);
	}

	this->unbind();
}