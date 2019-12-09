#pragma once
//-----
#include "axis.hpp"
#include "grid.hpp"
#include "graphicobjectg.hpp"
#include "graphicobjectc.hpp"
//----
#define BASICOBJECTVARIABLES GLenum primitiveType
#define OBJECTVARIABLES BASICOBJECTVARIABLES; std::vector<GVERTEX>* vertices = nullptr; std::vector<GLuint>* indices = nullptr; GraphicObjectG* object = nullptr
//---
//--
class ComparatorObjectID
{
	public:
		GLboolean operator()(_IN_(GraphicObject* object1), _IN_(GraphicObject* object2))
		{
			return object1->getObjectId() < object2->getObjectId();
		}
};

class ObjectManager
{
	public:
		enum class DIMENSION : GLubyte {Object2D, Object3D};

		static Axis* createAxis(_IN_(GLfloat & length), ObjectManager::DIMENSION dimension);
		static Axis* createTriangleAxis(_IN_(GLfloat & size), _IN_(GLfloat & offset));
		static Grid* createGrid(_IN_(GLubyte & lineCount));

		static GraphicObjectG* createBasicCube(_IN_(GLfloat & size), _IN_(GLfloat & offset), ObjectManager::DIMENSION dimension);

		ObjectManager();
		~ObjectManager();

		GraphicObjectG* createPrimitive(Object::OBJECTTYPE primitive, _IN_(std::basic_string<TCHAR> & objectName), _IN_(std::vector<GLfloat> & parameters));

		GLvoid setObjectId(_INOUT_(GraphicObject* object));

		inline GLvoid reset()
		{
			this->id = 0;
		}

		GLvoid load(_OUT_(std::vector<GraphicObject*> & objects), _IN_(rapidxml::xml_node<TCHAR>* parentNode));

	private:
		static GraphicObjectG* createObject(Object::OBJECTTYPE primitive, _IN_(std::vector<GLfloat> & parameters));

		static GraphicObjectG* loadGobject(_IN_(rapidxml::xml_node<TCHAR>* parentNode));
		static GraphicObjectC* loadCobject(_IN_(rapidxml::xml_node<TCHAR>* parentNode));

		GLuint id;
};