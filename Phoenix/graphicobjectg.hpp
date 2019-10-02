#pragma once
//-----
#include "graphicobject.hpp"
//----
//---
//--
class GraphicObjectG : public GraphicObject
{
	public:
		static const std::array<GLfloat, 4> lineColor;

		GraphicObjectG(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType), _IN_(std::vector<GLuint>* indices), Object::OBJECTTYPE type = Object::OBJECTTYPE::NOOBJECTTYPE, std::array<GLfloat, 4> parameters = std::array<GLfloat, 4>{0.0f, 0.0f, 0.0f, 0.0f});
		~GraphicObjectG();

		GLvoid bindEbo();

		inline GLvoid setTexture(_IN_(GLuint & texture))
		{
			this->texture = texture;
		}

		inline GLuint getTexture() const
		{
			return this->texture;
		}

		GLvoid drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode, GLboolean fixedPipeline = GL_FALSE);

		GraphicObject* clone();

		const Object::OBJECTTYPE type;
		const std::array<GLfloat, 4> parameters;

	private:
		//element buffer object
		GLuint ebo;

		const std::vector<GLuint>* indices;

		GLuint texture;
};