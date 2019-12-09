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
	GraphicObjectG* object = ObjectManager::createObject(primitive, parameters);

	if(object != nullptr)
	{
		this->setObjectId(object);

		object->setObjectName(objectName);
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

GLvoid ObjectManager::load(_OUT_(std::vector<GraphicObject*> & objects), _IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	if(parentNode != nullptr)
	{
		std::basic_string<TCHAR> type;
		GraphicObject* object = nullptr;
		rapidxml::xml_node<TCHAR>* childNode = nullptr;

		rapidxml::xml_node<TCHAR>* node = parentNode->first_node();

		while(node != nullptr)
		{
			if(FileIO::Import::isTag(_T("object"), node))
			{
				childNode = node->first_node(_T("type"));

				if(childNode != nullptr)
				{
					type = childNode->value();

					if(type.compare(_T("G")) == 0)
					{
						object = ObjectManager::loadGobject(node);
					}
					else if(type.compare(_T("C")) == 0)
					{
						object = ObjectManager::loadCobject(node);
					}
					else
					{
						object = nullptr;
					}

					if(object != nullptr)
					{
						*object << node;

						ExceptionHandler::push_back<GraphicObject*>(objects, object);
					}
				}
			}

			node = node->next_sibling();
		}

		std::vector<GraphicObject*>::const_iterator location = std::max_element(objects.begin(), objects.end(), ComparatorObjectID());

		if(location != objects.end())
		{
			this->id = (*location)->getObjectId();
		}
	}
}

GraphicObjectG* ObjectManager::createObject(Object::OBJECTTYPE primitive, _IN_(std::vector<GLfloat> & parameters))
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
	}

	return object;
}

GraphicObjectG* ObjectManager::loadGobject(_IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	std::vector<GLfloat> parameters(4, 0.0f);
	Object::OBJECTTYPE primitive = Object::OBJECTTYPE::NOOBJECTTYPE;

	rapidxml::xml_node<TCHAR>* node = parentNode->first_node(_T("primitive"));

	if(node != nullptr)
	{
		primitive = Object::convertObjectType(_ttoi(node->value()));
	}

	node = parentNode->first_node(_T("parameters"));

	if(node != nullptr)
	{
		Input::list2NumArray(node->value(), ' ', parameters);
	}

	if(primitive != Object::OBJECTTYPE::NOOBJECTTYPE)
	{
		if(!parameters.empty())
		{
			if(Input::checkFloatArrayValues(parameters, Object3D::minMax))
			{
				return ObjectManager::createObject(primitive, parameters);
			}
		}
	}

	return nullptr;
}

GraphicObjectC* ObjectManager::loadCobject(_IN_(rapidxml::xml_node<TCHAR>* parentNode))
{
	carve::input::Options options;
	carve::input::PolyhedronData data;
	carve::mesh::MeshSet<3>* meshSet = nullptr;
	GraphicObjectC* object = nullptr;
	rapidxml::xml_node<TCHAR>* childNode = nullptr;

	rapidxml::xml_node<TCHAR>* node = parentNode->first_node(_T("vertices"));

	if(node != nullptr)
	{
		std::array<GLfloat, 3> values;

		childNode = node->first_node();

		while(childNode != nullptr)
		{
			if(FileIO::Import::isTag(_T("v"), childNode))
			{
				Input::list2NumArray(childNode->value(), ' ', values);

				data.addVertex(carve::geom::VECTOR(values[0], values[1], values[2]));
			}

			childNode = childNode->next_sibling();
		}
	}

	node = parentNode->first_node(_T("faces"));

	if(node != nullptr)
	{
		std::vector<GLuint> values;

		childNode = node->first_node();

		while(childNode != nullptr)
		{
			if(FileIO::Import::isTag(_T("f"), childNode))
			{
				Input::list2Num(childNode->value(), ' ', values);

				data.addFace(values.begin(), values.end());
			}

			childNode = childNode->next_sibling();
		}
	}

	meshSet = data.createMesh(options);

	if(meshSet != nullptr)
	{
		carve::interpolate::FaceVertexAttr<TSTRUCT> fv_tex;
		carve::interpolate::FaceAttr<GLuint> f_tex_num;

		object = new GraphicObjectC(meshSet, fv_tex, f_tex_num);
	}

	return object;
}