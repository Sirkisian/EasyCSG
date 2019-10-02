#include "graphicobject.hpp"

const std::array<GLfloat, 4> GraphicObject::defaultObjectColor{0.4f, 0.3f, 0.3f, 1.0f};
const std::array<GLfloat, 4> GraphicObject::selectedObjectColor{0.3f, 0.3f, 0.4f, 1.0f};

GraphicObject::GraphicObject():
SceneObject(),
id(0)
{
	this->objectColor = defaultObjectColor;
}

GraphicObject::GraphicObject(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType)):
SceneObject(vertices, primitiveType),
id(0)
{
	this->objectColor = defaultObjectColor;
}

GraphicObject::~GraphicObject()
{
}