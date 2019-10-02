#pragma once
//-----
#include "sceneobject.hpp"
//----
//---
//--
class Axis : public SceneObject
{
	public:
		static const GLfloat alpha;

		Axis(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType));
		~Axis();

		inline GLvoid setLineWidth(GLfloat lineWidth)
		{
			this->lineWidth = lineWidth;
		}

		GLvoid drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode = SceneObject::DRAWMODE::COLOR, GLboolean fixedPipeline = GL_FALSE);

		private:
			GLfloat lineWidth;
};