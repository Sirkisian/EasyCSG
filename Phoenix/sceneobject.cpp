#include "sceneobject.hpp"

SceneObject::SceneObject():
vertices(nullptr),
primitiveType(0)
{
}

SceneObject::SceneObject(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType)):
vertices(vertices),
primitiveType(primitiveType)
{
}

SceneObject::~SceneObject()
{
	if(this->vertices != nullptr)
	{
		delete this->vertices;
		this->vertices = nullptr;
	}
}

GLvoid SceneObject::bindVbo()
{
	if(this->vbo > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GVERTEX) * this->vertices->size(), this->vertices->data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GVERTEX), nullptr);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GVERTEX), reinterpret_cast<GLvoid*>((sizeof(GLfloat) * 3)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GVERTEX), reinterpret_cast<GLvoid*>((sizeof(GLfloat) * 6)));
		glEnableVertexAttribArray(2);
	}
}

GLvoid SceneObject::setShaderModelVar(_IN_(GLuint & shaderProgram))
{
	glm::mat4 model = glm::mat4(1.0f);
	this->transformation.multModelMatrix(model);

	GLfloat* pFloat = glm::value_ptr(model);
	std::vector<GLfloat> matrix(pFloat, pFloat + 16);

	ShaderProgram::setMatrix(shaderProgram, std::basic_string<GLchar>("model"), matrix);

	model = glm::inverse(model);
	model = glm::transpose(model);
	glm::mat3 transposeInverseModel = glm::mat3(model);

	pFloat = glm::value_ptr(transposeInverseModel);
	matrix = std::vector<GLfloat>(pFloat, pFloat + 9);

	ShaderProgram::setMatrix(shaderProgram, std::basic_string<GLchar>("transposeInverseModel"), matrix);
}