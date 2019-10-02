#pragma once
//-----
#include "objectmanager.hpp"
//----
//---
//--
class TransformationAxis
{
	public:
		TransformationAxis(_IN_(GLfloat & axisLength), _IN_(GLfloat & triangleSize), _IN_(GLfloat & cubeSize));
		~TransformationAxis();

		GLvoid translateTransformationAxis(_IN_(long & x), _IN_(long & y));

		GLvoid drawTransformationAxis(_IN_(GLuint & shader));

		Axis* axis;
		Axis* triangleAxis;
		GraphicObjectG* cube;
};