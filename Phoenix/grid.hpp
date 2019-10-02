#pragma once
//-----
#include "sceneobject.hpp"
//----
//---
//--
class Grid : public SceneObject
{
	public:
		static const std::array<GLfloat, 4> gridColor;

		Grid(_IN_(std::vector<GVERTEX>* vertices), _IN_(GLenum & primitiveType));
		~Grid();

		GLvoid drawObject(_IN_(GLuint & shaderProgram), SceneObject::DRAWMODE mode = SceneObject::DRAWMODE::COLOR, GLboolean fixedPipeline = GL_FALSE);
};