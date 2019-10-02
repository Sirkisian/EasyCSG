#include "transformation.hpp"

Transformation::Transformation()
{
	this->translation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

Transformation::~Transformation()
{
}

GLvoid Transformation::multModelMatrix(_INOUT_(glm::mat4 & matrix)) const
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), this->translation);
	model = glm::rotate(model, glm::radians(this->rotation[mCOORDINATE::X]), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(this->rotation[mCOORDINATE::Y]), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(this->rotation[mCOORDINATE::Z]), glm::vec3(0, 0, 1));
	model = glm::scale(model, this->scaling);

	matrix *= model;
}

GLvoid Transformation::transform(_INOUT_(glm::vec3 & transformation), _IN_(GLfloat & x), _IN_(GLfloat & y), _IN_(GLfloat & z), _IN_(Transformation::ACTION & action))
{
	switch(action)
	{
		case Transformation::ACTION::ADD:
		{
			transformation[mCOORDINATE::X] += x;
			transformation[mCOORDINATE::Y] += y;
			transformation[mCOORDINATE::Z] += z;
		}
		break;

		case  Transformation::ACTION::SET:
		{
			transformation[mCOORDINATE::X] = x;
			transformation[mCOORDINATE::Y] = y;
			transformation[mCOORDINATE::Z] = z;
		}
		break;
	}
}