#include "opencsgobject.hpp"

OpenCsgObject::OpenCsgObject(OpenCSG::Operation operation, unsigned int convexity, GraphicObject* graphicObject):
OpenCSG::Primitive(operation, convexity),
shaderProgram(0),
graphicObject(graphicObject)
{
}

OpenCsgObject::~OpenCsgObject()
{
}

void OpenCsgObject::render()
{
	this->graphicObject->drawObject(this->shaderProgram, SceneObject::DRAWMODE::CSG, true);
}