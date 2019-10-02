#include "transformationaxis.hpp"

TransformationAxis::TransformationAxis(_IN_(GLfloat & axisLength), _IN_(GLfloat & triangleSize), _IN_(GLfloat & cubeSize))
{
	this->axis = ObjectManager::createAxis(axisLength, ObjectManager::DIMENSION::Object2D);

	this->triangleAxis = ObjectManager::createTriangleAxis(triangleSize, axisLength);

	this->cube = ObjectManager::createBasicCube(cubeSize, cubeSize / 3.0f, ObjectManager::DIMENSION::Object2D);
}

TransformationAxis::~TransformationAxis()
{
	if(this->axis != nullptr)
	{
		delete this->axis;
		this->axis = nullptr;
	}

	if(this->triangleAxis != nullptr)
	{
		delete this->triangleAxis;
		this->triangleAxis = nullptr;
	}

	if(this->cube != nullptr)
	{
		delete this->cube;
		this->cube = nullptr;
	}
}

GLvoid TransformationAxis::translateTransformationAxis(_IN_(long & x), _IN_(long & y))
{
	if(this->axis != nullptr)
	{
		this->axis->transformation.translate(static_cast<GLfloat>(x), static_cast<GLfloat>(y), 0, Transformation::ACTION::SET);
	}

	if(this->triangleAxis != nullptr)
	{
		this->triangleAxis->transformation.translate(static_cast<GLfloat>(x), static_cast<GLfloat>(y), 0, Transformation::ACTION::SET);
	}

	if(this->cube != nullptr)
	{
		this->cube->transformation.translate(static_cast<GLfloat>(x), static_cast<GLfloat>(y), 0, Transformation::ACTION::SET);
	}
}

GLvoid TransformationAxis::drawTransformationAxis(_IN_(GLuint & shader))
{
	if(this->axis != nullptr)
	{
		this->axis->drawObject(shader);
	}

	if(this->triangleAxis != nullptr)
	{
		this->triangleAxis->drawObject(shader);
	}

	if(this->cube != nullptr)
	{
		this->cube->drawObject(shader, SceneObject::DRAWMODE::COLOR);
	}
}