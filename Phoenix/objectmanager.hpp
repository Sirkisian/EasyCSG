#pragma once
//-----
#include "axis.hpp"
#include "grid.hpp"
#include "graphicobjectg.hpp"
//----
#define BASICOBJECTVARIABLES GLenum primitiveType
#define OBJECTVARIABLES BASICOBJECTVARIABLES; std::vector<GVERTEX>* vertices = nullptr; std::vector<GLuint>* indices = nullptr; GraphicObjectG* object = nullptr
//---
//--
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

	private:
		GLuint id;
};