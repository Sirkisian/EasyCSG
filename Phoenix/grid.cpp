#include "grid.hpp"

const std::array<GLfloat, 4> Grid::gridColor{0.8f, 0.6f, 0.7f, 1.0f};

Grid::Grid(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType)):
SceneObject(vertices, primitiveType)
{
	if(this->bindVao())
	{
		this->bindVbo();
		this->unbind();
	}
}

Grid::~Grid()
{
}

GLvoid Grid::drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode, GLboolean fixedPipeline)
{
	if(!this->bindVao())
	{
		this->bindVbo();
	}

	this->setShaderModelVar(shaderProgram);

	ShaderProgram::setVector<4>(shaderProgram, std::basic_string<GLchar>("color"), &Grid::gridColor);

	GLuint loop = this->vertices->size() / 2;
	for(GLuint i = 0; i < loop; i++)
	{
		if((i % 2) == 0)
		{
			glLineWidth(1.5f);
		}
		else
		{
			glLineWidth(0.5f);
		}

		glDrawArrays(this->primitiveType, 2 * i, 2);
	}

	this->unbind();
}