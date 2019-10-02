#pragma once
//-----
#include "material.hpp"
#include "sceneobject.hpp"
//----
//---
//--
class GraphicObject : public SceneObject
{
	public:
		static const GLubyte objectNameSize = 25;
		static const std::array<GLfloat, 4> defaultObjectColor;
		static const std::array<GLfloat, 4> selectedObjectColor;

		GraphicObject();
		GraphicObject(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType));
		virtual ~GraphicObject();

		inline GLvoid setObjectId(_IN_(GLuint & id))
		{
			this->id = id;
		}

		inline const GLuint & getObjectId() const
		{
			return this->id;
		}

		inline GLvoid setObjectName(_IN_(std::basic_string<TCHAR> & objectName))
		{
			this->objectName = objectName.substr(0, GraphicObject::objectNameSize);
		}

		inline GLvoid setObjectColor(_IN_(ARRAY4REF(GLfloat, objectColor)))
		{
			this->objectColor = objectColor;
		}

		inline GLvoid setObjectMaterial(_IN_(ARRAY4REF(GLfloat, materialValues)), Material::MATERIALTYPE materialType)
		{
			this->material.setMaterialValues(materialValues, materialType);
		}

		inline std::basic_string<TCHAR> getObjectName() const
		{
			return this->objectName;
		}

		virtual GLvoid drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode, GLboolean fixedPipeline = GL_FALSE) = 0;

		virtual GraphicObject* clone() = 0;

		Material material;

	protected:
		GLuint id;
		std::array<GLfloat, 4> objectColor;
		std::basic_string<TCHAR> objectName;
};