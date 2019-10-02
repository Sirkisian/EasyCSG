#include "objectmanager.hpp"

Axis* ObjectManager::createAxis(_IN_(GLfloat & length), ObjectManager::DIMENSION dimension)
{
	BASICOBJECTVARIABLES;
	Axis* axis = nullptr;
	std::vector<GVERTEX>* vertices = nullptr;

	switch(dimension)
	{
		case ObjectManager::DIMENSION::Object2D:
			{
				vertices = Object2D::axis(length, primitiveType);
			}
			break;

		case ObjectManager::DIMENSION::Object3D:
			{
				vertices = Object3D::axis(length, primitiveType);
			}
			break;
	}

	if(vertices != nullptr)
	{
		axis = new Axis(vertices, primitiveType);
	}

	return axis;
}

Axis* ObjectManager::createTriangleAxis(_IN_(GLfloat & size), _IN_(GLfloat & offset))
{
	BASICOBJECTVARIABLES;
	Axis* axis = nullptr;

	std::vector<GVERTEX>* vertices = Object2D::triangleAxis(size, offset, primitiveType);

	if(vertices != nullptr)
	{
		axis = new Axis(vertices, primitiveType);
	}

	return axis;
}

Grid* ObjectManager::createGrid(_IN_(GLubyte & lineCount))
{
	BASICOBJECTVARIABLES;
	Grid* grid = nullptr;

	std::vector<GVERTEX>* vertices = Object3D::grid(lineCount, primitiveType);

	if(vertices != nullptr)
	{
		grid = new Grid(vertices, primitiveType);
	}

	return grid;
}

GraphicObjectG* ObjectManager::createBasicCube(_IN_(GLfloat & size), _IN_(GLfloat & offset), ObjectManager::DIMENSION dimension)
{
	OBJECTVARIABLES;

	switch(dimension)
	{
		case ObjectManager::DIMENSION::Object2D:
		{
			vertices = Object2D::basicCube(size, offset, primitiveType, &indices);
		}
		break;

		case ObjectManager::DIMENSION::Object3D:
		{
			vertices = Object3D::basicCube(size, primitiveType, &indices);
		}
		break;
	}

	if(vertices != nullptr)
	{
		object = new GraphicObjectG(vertices, primitiveType, indices);
	}

	return object;
}

ObjectManager::ObjectManager():
id(0)
{
}

ObjectManager::~ObjectManager()
{
}

GraphicObjectG* ObjectManager::createPrimitive(Object::OBJECTTYPE primitive, _IN_(std::basic_string<TCHAR> & objectName), _IN_(std::vector<GLfloat> & parameters))
{
	OBJECTVARIABLES;

	switch(primitive)
	{
		case Object::OBJECTTYPE::CUBE:
			{
				vertices = Object3D::primitiveCube(parameters[0], parameters[1], parameters[2], primitiveType, &indices);
			}
			break;

		case Object::OBJECTTYPE::SPHERE:
			{
				vertices = Object3D::primitiveSphere(parameters[0], static_cast<GLushort>(parameters[1]), static_cast<GLushort>(parameters[2]), primitiveType, &indices);
			}
			break;

		case Object::OBJECTTYPE::CYLINDER:
			{
				vertices = Object3D::primitiveCylinder(parameters[0], parameters[1], static_cast<GLushort>(parameters[2]), primitiveType, &indices);
			}
			break;
	}

	if(vertices != nullptr)
	{
		std::array<GLfloat, 4> arrayParameters;
		std::copy(parameters.begin(), parameters.end(), arrayParameters.begin());

		object = new GraphicObjectG(vertices, primitiveType, indices, primitive, arrayParameters);

		if(object != nullptr)
		{
			this->setObjectId(object);

			object->setObjectName(objectName);
		}
	}

	return object;
}

GLvoid ObjectManager::setObjectId(_INOUT_(GraphicObject* object))
{
	if(object != nullptr)
	{
		this->id++;
		object->setObjectId(this->id);
	}
}