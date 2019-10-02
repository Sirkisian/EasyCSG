#pragma once
//-----
#include <gl/glew.h>
//----
//---
//--
class DrawnObject
{
	public:
		DrawnObject();
		virtual ~DrawnObject();

	protected:
		GLboolean bindVao();
		virtual GLvoid bindVbo() = 0;
		GLvoid unbind();

		//vertex array object
		GLuint vao;
		//vertex buffer object
		GLuint vbo;
};