#pragma once
//-----
#include "enumconverter.hpp"
#include "drawnobject.hpp"
#include "object.hpp"
#include "transformation.hpp"
#include "shaderprogram.hpp"
//----
//---
//--
class SceneObject : public DrawnObject
{
	public:
		enum class DRAWMODE : GLubyte {NODRAWMODE, WIRE, COLOR, LIGHTING, TEXTURE, CSG, SELECT};

		static inline SceneObject::DRAWMODE convertDrawMode(GLubyte drawMode)
		{
			return EnumConverter::convert<SceneObject::DRAWMODE>(drawMode, SceneObject::DRAWMODE::WIRE, SceneObject::DRAWMODE::SELECT);
		}

		SceneObject();
		SceneObject(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType));
		virtual ~SceneObject();

		virtual GLvoid drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode, GLboolean fixedPipeline = GL_FALSE) = 0;

		Transformation transformation;

	protected:
		GLvoid bindVbo();

		GLvoid setShaderModelVar(_IN_(GLuint & shaderProgram));

		GLenum primitiveType;
		const std::vector<GVERTEX>* vertices;
};